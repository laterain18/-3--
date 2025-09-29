# 导入必要模块
import network
import socket
import time
import _thread  # 用于多线程处理
import os
import gc  # 垃圾回收
from machine import Pin
from machine import FPIOA
from media.sensor import *  # 摄像头模块
from media.media import *  # 媒体管理器

# WiFi 配置
SSID = "team"
PASSWORD = "12345qwert"

# 全局变量，便于清理
sensor = None
server = None
sta = None
running = True  # 控制线程运行的标志
gpio = None
fpioa = None

# WiFi 连接函数
def connect_wifi():
    global sta
    sta = network.WLAN(network.STA_IF)
    if not sta.active():
        sta.active(True)
    print("正在连接 WiFi:", SSID)
    sta.connect(SSID, PASSWORD)
    max_wait = 15  # 增加超时
    while max_wait > 0:
        print("WiFi 状态:", sta.status(), "连接中...")
        if sta.isconnected():
            ip_info = sta.ifconfig()
            ip = ip_info[0]
            if ip != '0.0.0.0':
                print("WiFi 连接成功！IP 地址:", ip)
                return ip
            print("IP 无效，重试...")
        max_wait -= 1
        time.sleep(1)
    print("WiFi 连接失败，请检查 SSID/密码")
    raise SystemExit

# 连接 WiFi
try:
    device_ip = connect_wifi()
except SystemExit:
    print("程序因 WiFi 连接失败终止")
    raise

# 初始化摄像头
try:
    sensor = Sensor(id=2)  # 使用 CSI2 接口（GC2093）
    sensor.reset()
    print("传感器初始化成功 (GC2093 CSI2)")
    sensor.set_framesize(Sensor.QVGA)  # 优化：320x240，低分辨率提高 FPS
    sensor.set_pixformat(Sensor.RGB565)  # 修复：RGB565 支持
    MediaManager.init()  # 初始化媒体管理器
    sensor.run()  # 启动摄像头
except Exception as e:
    print("摄像头初始化失败:", e)
    raise SystemExit

# GPIO 初始化
try:
    fpioa = FPIOA()
    fpioa.set_function(52, FPIOA.GPIO0)
    gpio = Pin(52, Pin.OUT, pull=Pin.PULL_NONE, drive=7)
    gpio.value(0)  # 默认低电平
except Exception as e:
    print("GPIO 初始化失败:", e)
    raise SystemExit

# 缓冲队列
raw_frame_queue = []
queue_lock = _thread.allocate_lock()

# 捕获与处理线程
def capture_thread():
    global raw_frame_queue, running
    last_time = time.ticks_ms()
    frame_count = 0
    # 颜色阈值
    color_thresholds = [
        (0, 20, -10, 10, -10, 10),  # 黑色块
        (30, 70, 20, 80, -10, 10)   # 红色块
    ]
    while running:
        try:
            img = sensor.snapshot()  # 捕获原始图像
            jpeg_raw = img.compressed(quality=50)  # 压缩原始图像
            with queue_lock:
                if len(raw_frame_queue) < 3:
                    raw_frame_queue.append(jpeg_raw)

            # 颜色识别，控制 GPIO
            blobs = img.find_blobs(color_thresholds, area_threshold=2000)
            if blobs:
                gpio.value(1)  # 检测到色块，置高电平
                for blob in blobs:
                    threshold_index = blob[8]
#                    print("色块中心: X={}, Y={}, 颜色={}".format(blob[5], blob[6], "黑色" if threshold_index == 0 else "红色"))
            else:
                gpio.value(0)  # 未检测到色块，置低电平

            with queue_lock:
                frame_count += 1
                if frame_count % 10 == 0:
                    current_time = time.ticks_ms()
                    fps = 10000 / time.ticks_diff(current_time, last_time)
                    print("捕获 FPS:", fps, "GPIO状态:", "高电平" if gpio.value() == 1 else "低电平")
                    last_time = current_time
                    gc.collect()  # 每10帧清理内存
        except Exception as e:
            print("捕获错误:", e)

# 启动捕获线程
_thread.start_new_thread(capture_thread, ())

# MJPEG 处理函数（原始图像）
def mjpeg_handler(client):
    try:
        client.sendall(b'HTTP/1.0 200 OK\r\n')
        client.sendall(b'Server: K230 MJPEG Streamer\r\n')
        client.sendall(b'Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n')
        fps_count = 0
        start_time = time.ticks_ms()
        while running:
            os.exitpoint()  # 允许外部中断
            with queue_lock:
                if raw_frame_queue:
                    jpeg = raw_frame_queue.pop(0)
                else:
                    time.sleep(0.01)
                    continue
            client.sendall(b'--frame\r\n')
            client.sendall(b'Content-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n' % len(jpeg))
            client.sendall(jpeg)
            client.sendall(b'\r\n')
            fps_count += 1
            if time.ticks_diff(time.ticks_ms(), start_time) > 1000:
                print("发送 FPS:", fps_count)
                fps_count = 0
                start_time = time.ticks_ms()
    except Exception as e:
        print("客户端异常:", e)
    finally:
        client.close()
        print("客户端连接关闭")

# 创建服务器
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(1)  # 确保阻塞模式
server.bind(('', 8080))  # 绑定所有接口，端口 8080
server.listen(5)  # 允许最多 5 个等待连接
print("MJPEG 服务器启动，访问: http://" + device_ip + ":8080")

# 主循环
try:
    while running:
        os.exitpoint()
        try:
            client, addr = server.accept()
            print("客户端连接:", addr)
            _thread.start_new_thread(mjpeg_handler, (client,))
        except OSError as e:
            if e.errno == 11:  # EAGAIN
                print("EAGAIN 错误，稍后重试:", e)
                time.sleep(0.2)
            else:
                raise
except KeyboardInterrupt:
    print("用户中断")
    running = False
except Exception as e:
    print("服务器异常:", e)
finally:
    try:
        sensor.stop()
        server.close()
        sta.disconnect()  # 修复 EPERM
        sta.active(False)
        MediaManager.deinit()
        gc.collect()  # 额外内存清理
        os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
        time.sleep_ms(100)
        print("程序结束，所有资源已清理")
    except OSError as e:
        print("清理时 EPERM 错误:", e, " - 请重启硬件")
