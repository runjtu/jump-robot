from scipy.spatial.distance import euclidean  # 用来计算端点之间的欧氏距离
import numpy as np
import imutils
import time
import cv2


def set_camera_type():
    while True:
        try:
            set_type = int(input('摄像头调用（输入数字代号：0.内置，1.外置）：'))
        except ValueError:
            #delay('输入参数类型错误')
            continue
        else:
            if (set_type < 0) or (set_type > 1):
                #delay('输出参数不在范围内')
                continue
            elif set_type == 0:
                print('选择：内置摄像头')
            else:
                print('选择：外置摄像头')
            break
    return set_type

def call_camera():
    #video = "http://192.168.137.170:8080/?action=stream"
    #camera = cv2.VideoCapture(camera_type, cv2.CAP_DSHOW)
    camera = cv2.VideoCapture(camera_type)
    if camera.isOpened() is False:
        print('摄像头调用失败')
        raise AssertionError
    else:
        while True:
            frame = camera.read()[1]  # 返回捕获到的RGB
            image = cv2.flip(frame, 1, dst=None)
            cv2.imshow('Camera', image)
            if cv2.waitKey(1) == ord('q'):
                cv2.destroyWindow('Camera')
                break
            #if (cv2.waitKey(1) > -1) or (cv2.getWindowProperty('Camera', cv2.WND_PROP_VISIBLE) < 1.0):  # 设置关闭条件
               #cv2.destroyWindow('Camera')
                #break
    return image

def get_points(image):
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    gaussian_blur = cv2.GaussianBlur(gray_image, (5, 5), 0)
    min_val, max_val = 50, 100
    margin = cv2.Canny(gaussian_blur, min_val, max_val)
    open_margin = cv2.dilate(margin, None, iterations=20)  # 开运算，如果有纯色平台iteration可以小一些
    contours = cv2.findContours(open_margin, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    points = imutils.grab_contours(contours)
    return points

def draw_frame(image, points, tag):
    if tag == 0:
        for point in points:
            min_area = cv2.minAreaRect(point)
            min_area_point = cv2.boxPoints(min_area)  # 获取最小外接矩阵的四个端点
            int_point = [min_area_point.astype('int')]
            cv2.drawContours(image, int_point, -1, (0, 0, 255), 1)
            return min_area_point
    else:
        for point in points:
            min_area = cv2.minAreaRect(point)
            min_area_point = cv2.boxPoints(min_area)
            left_point, right_point = min_area_point[0], min_area_point[1]
            botten = min_area_point[3]
            chang = (right_point[0] - left_point[0])/rate
            gao = (botten[1]-right_point[1])/rate
            X = int(left_point[0])  # 获取顶部中点X坐标
            Y = int(left_point[1])  # 获取顶部中点Y坐标
            int_point = [min_area_point.astype('int')]
            cv2.drawContours(image, int_point, -1, (0, 0, 255), 1)  # 绘制边框
            radius = (euclidean(left_point, right_point) / 2) / rate  # 获取半径
            area = int((3.1415926 * pow(radius, 2)))
            # 展示面积信息
            cv2.putText(image, '{} {:.2f} {} {:.2f}'.format('width:',chang,'high:',gao), (int(X), int(Y)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 1)

def rate_calculation():
    #delay('计算比率')
    left_point, right_point = reference_points[0], reference_points[1]
    length_euclidean = euclidean(left_point, right_point)  # 计算欧氏距离
    while True:
        try:
            length_reference = int(input('输入参照物长度(mm)：'))
        except ValueError:
            #delay('输入参数类型错误')
            continue
        else:
            if length_reference <= 0:
                #delay('参数不可小于或等于0')
                continue
            else:
                break
    rate = length_euclidean / length_reference  # 比率计算
    print('(参照物)欧氏长度：{}mm'.format(length_euclidean))
    print('(参照物)实际长度：{}mm'.format(length_reference))
    print('长度比率：{}'.format(rate))
    return rate
def reference_processing():
    circulation = True
    while circulation:
        image = call_camera()
        points = get_points(image)  # 图像处理
        selected_points = []  # 创建被筛选的轮廓数据的容器
        filter_area = 1
        while True:
            [selected_points.append(i) for i in points if cv2.contourArea(i) > filter_area]
            if len(selected_points) > 1:
                selected_points.clear()  # 清空内容，为下一次存储数据用
                filter_area += 1
            else:
                break
        reference_area_point = draw_frame(image, selected_points, 0)
        while True:
            cv2.imshow('reference', image)
            if cv2.waitKey(1) == ord('q'):
                cv2.destroyWindow('reference')
                break
            #if (cv2.waitKey(1) > -1) or (cv2.getWindowProperty('reference', cv2.WND_PROP_VISIBLE) < 1.0):
                #cv2.destroyWindow('reference')
                #break
        while circulation:
            try:
                tag = str(input('是否是理想参照物(Y/N)：'))
            except ValueError:
                #delay('输入参数类型错误')
                continue
            else:
                if (tag == 'Y') or (tag == 'y'):
                    circulation = False
                    break
                elif (tag == 'N') or (tag == 'n'):
                    break
    return filter_area, reference_area_point


def real_time_processing():
    print('进入实时测量，按下q键结束程序')
    #camera = cv2.VideoCapture(camera_type, cv2.CAP_DSHOW)
    camera = cv2.VideoCapture(camera_type)
    while True:
        frame = camera.read()[1]
        image = cv2.flip(frame, 1, dst=None)
        points = get_points(image)  # 获取所有参照物的端点
        selected_points = []
        [selected_points.append(i) for i in points if cv2.contourArea(i) > filter_area]  # 筛选后的端点

        #length_euclidean
        #dist.euclidean
        #cv2.FONT_HERSHEY_SIMPLEX
        draw_frame(image, selected_points, 1)  # 绘制边框
        cv2.imshow('Camera', image)
        if cv2.waitKey(1) == ord('q'):
                cv2.destroyWindow('Camera')
                break
        #if (cv2.waitKey(1) > -1) or (cv2.getWindowProperty('Camera', cv2.WND_PROP_VISIBLE) < 1.0):
            #cv2.destroyWindow('Camera')
            #break


if __name__ == '__main__':
    camera_type = set_camera_type()  # 设置相机类型
    filter_area, reference_points = reference_processing()  # 创建被过滤面积值
    rate = rate_calculation()  # 计算欧氏距离与实际距离的比率
    real_time_processing()  # 实现实时测量
