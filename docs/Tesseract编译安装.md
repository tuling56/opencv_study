## 编译安装Tesseract

### 安装依赖项

> leptonica

```shell
wget http://www.leptonica.com/source/leptonica-1.72.tar.gz
tar -xvf leptonica-1.72.tar.gz  
cd leptonica-1.72
./configure && make && make instal
```

> automake、libtool

```
yum install automake libtool
```

### 安装Tesseract

```shell
wget https://github.com/tesseract-ocr/tesseract/archive/3.04.00.tar.gz
mv 3.04.00  Tesseract3.04.00.tar.gz
tar -xvf Tesseract3.04.00.tar.gz  
cd tesseract-3.04.00/
./autogen.sh
./configure --prefix=/usr/local/tesseract
make && make install
```

​	注意事项：在配置完后

> Configuration is done.
> You can now build and install tesseract by running:
>
> $ make
> $ sudo make install
>
> Training tools can be build and installed (after building of tesseract) with:
>
> $ make training
> $ sudo make training-install
>
> > 会在/usr/local/tesseract/bin目录下多出训练数据的的工具
> >
> > /usr/local/tesseract/bin/ambiguous_words
> > /usr/local/tesseract/bin/classifier_tester
> > /usr/local/tesseract/bin/cntraining
> > /usr/local/tesseract/bin/combine_tessdata
> > /usr/local/tesseract/bin/dawg2wordlist
> > /usr/local/tesseract/bin/mftraining
> > /usr/local/tesseract/bin/set_unicharset_properties
> > /usr/local/tesseract/bin/shapeclustering
> > /usr/local/tesseract/bin/text2image
> > /usr/local/tesseract/bin/unicharset_extractor
> > /usr/local/tesseract/bin/wordlist2dawg

### 配置

> 下载字体库

```shell
cd /usr/local/tesseract/share/tessdata
wget --no-check-certificate https://github.com/tesseract-ocr/tessdata/raw/master/eng.traineddata
wget --no-check-certificate https://github.com/tesseract-ocr/tessdata/raw/master/chi_sim.traineddata 
```

> 环境变量设置

```
vim ~/.bash_profile
export TESS_ROOT=/usr/local/tesseract
export TESSDATA_PREFIX=/usr/local/tesseract/share
export PATH=$PATH:$TESS_ROOT/bin

# 可选
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/tesseract/lib   		  # 添加查找库的路径
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/usr/local/tesseract/lib/pkgconfig  # 添加pkgconfig的查找路径
```

> 测试

```shell
convert -negate card.jpg card.tif
tesseract card.tif ./b  -psm 3 -l chi_sim+eng   
```

### 参考

[CENTOS 下 编译安装 tesseract-ocr 3.0.4 识别文字](http://www.cnblogs.com/dajianshi/p/4932882.html)

