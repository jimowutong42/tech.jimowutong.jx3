# -*- coding:utf-8 -*-

import io
import os
import time
import configparser
from PIL import Image
from selenium import webdriver

conf = configparser.ConfigParser()
curpath = os.path.dirname(os.path.realpath(__file__))
cfgpath = os.path.join(curpath, "config.ini")
conf.read(cfgpath)
path = conf.get('PATH', 'aliyun')
url = conf.get('URL', 'url')

options = webdriver.ChromeOptions()
options.binary_location = path
options.add_argument('''--no-sandbox''')
options.add_argument('''--disable-gpu''')
driver = webdriver.Chrome(r'chromedriver.exe', options=options)
driver.get(url)
# driver.implicitly_wait(2)  # 设置隐式等待时间
time.sleep(30)
# driver.find_element_by_id('blankpage-button-pc').click()  # 点击登录按钮
# time.sleep(3)
# driver.switch_to.frame(driver.find_element_by_id('login_frame'))
# time.sleep(1)
# driver.find_element_by_class_name('face').click()  # QQ快捷登录
# time.sleep(3)
# print(driver.title)
# driver.find_element_by_id("user_name").send_keys("fnngj")
# ActionChains(driver).move_by_offset(104, 213).click().perform()
# print(driver.find_element_by_class_name("formula-input").text)
# ActionChains(driver).move_by_offset(106, 213).click().perform()
# print(driver.find_element_by_class_name("formula-input").text)

while 1:
    png = driver.find_element_by_class_name('main-board').screenshot_as_png
    byte_stream = io.BytesIO(png)  # 把请求到的数据转换为Bytes字节流
    img = Image.open(byte_stream)
    # print(img.size)
    cropped = img.crop((0, 0, 535, 356))  # (left, upper, right, lower)
    # print(cropped.size)
    # matplotlib.pyplot.imshow(cropped)  # 显示图片
    cropped.save("../../../image/tech.jimowutong.jx3/chakanqiandaobiao.png")
    time.sleep(30)

driver.quit()
