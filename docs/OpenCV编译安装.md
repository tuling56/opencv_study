## OpenCV编译安装

### 安装

> 安装依赖项(可选)

```shell
1.安装编译环境
$ sudo dnf install gcc gcc-c++ ncurses-devel cmake

2.安装GTK+2.x
$ sudo dnf install gtk2 gtk2-devel gtk2-devel-docs
$ sudo dnf install libgnome-devel gnome-devel-docs
$ sudo dnf install zlib glib atk pango gdk-pixbuf2 gdk-pixbuf2-devel

3.安装各种需要的包
$ sudo dnf install gtk+ gtk2-devel pkgconfig Python-devel python-numpydoc                # 图形界面支持
$ sudo dnf install libavc1394 libavc1394-devel libdc1394 libdc1394-devel jasper-devel 		 # 视频支持
$ sudo dnf install ffmpeg ffmpeg-devel openjpeg-devel libjpeg-devel libpng-devel libtiff-devel#图像支持
```

> 安装OpenCV

```shell
cd ~/OpenCV 
mkdir release  
cd release 
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local/opencv -D BUILD_PYTHON_SUPPORT=ON ..
make 
sudo make install
```

### 配置

> 在~/.bash_profile中添加以下内容：
>
> export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig/:/usr/local/opencv/lib/pkgconfig/
>
> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/local/opencv/lib
>
> 然后执行：
>
> source ~/.bash_profile
>
> 说明：配置库的时候的另外一种方法
>
> >  sudo vim /etc/ld.so.conf.d/opencv.conf
> >
> > 添加内容：/usr/local/opencv/lib；然后执行 ldconfig

### 参考

 [Opencv Linux环境搭建](http://blog.csdn.net/tanhngbo/article/details/40383583)

 [在Fedora22中安装OpenCV3.0.0](http://blog.csdn.net/ws_20100/article/details/46493293)

