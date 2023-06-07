![JavaScript](https://img.shields.io/static/v1?style=flat&message=JavaScript&color=373e47&logo=JavaScript&logoColor=F7DF1E&label=)
![Firebase](https://img.shields.io/static/v1?style=flat&message=Arduino&color=373e47&logo=Arduino&logoColor=00979C&label=)

<p align="center">
  <img height="100" src="/img/car.png" />
</p>

### ESP32 setup

---

In the _Arduino IDE_ choose the _ESP32 Dev Module_ board. Paste the [carLibrary](carLibrary/) directory into your arduino library folder. Then install the following libraries and their dependencies via the library manager in the IDE.

-  AsyncTCP

-  Adafruit_SSD1306

Lastly install the [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer#installation) library as a _.ZIP_ file from github and include it in the IDE.

### ZUMO34u4 setup

---

Select _Arduino Leonardo_ as the board and download the following library and its dependencies using the library manager.

-  Zumo32U4
