#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2020-9-27 10:02:11
# @Author  : khz_ls
# @Site    :
# @File    : __init__.py.py
# @Software: ce9nt
import os


def message(message, data):
    if message["type"] == 'send':
        print(u"[***] {0}".format(message['payload']))
    else:
        print(message)


def main():
    import frida
    import sys
    jsCode = ""
    with open("frida注入测试.js", "r", encoding='utf-8') as f:
        jsCode = f.read()

    host = '4b0e004e'
    app_package_name = 'com.ninecents'
    manager = frida.get_device_manager()
    device = manager.get_device(host)
    process = device.attach(app_package_name)

    script = process.create_script(jsCode)
    script.on("message", message)
    script.load()
    sys.stdin.read()


if __name__ == "__main__":
    print("------------------    Enter __main__    ------------------")

    print(u"[Current work directory is : ]\t" + os.getcwd())
    print(u"[Current process ID is : ]\t" + str(os.getpid()))
    print("\n")
    main()

    print("------------------    Leave __main__    ------------------")
