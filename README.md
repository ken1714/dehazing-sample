# ヘイズ除去プログラム
## 1. 概要

静止画像を入力し、ヘイズを除去した画像を出力するプログラム。He[^he]の手法をベースに実装している。

|入力画像|出力画像|
|-------|-------|
|<img src="tests/data/original.png" width=320>|<img src="tests/data/dehazed.png" width=320>|

## 2. 実行環境とその構築

Dockerコンテナ上での実行を想定している。以降、その構成と環境構築方法を述べる。

### 2.1 Docker環境の構成

- ベースイメージ: [nvidia/cuda:11.3.1-cudnn8-devel-ubuntu20.04](https://hub.docker.com/layers/nvidia/cuda/11.3.1-cudnn8-devel-ubuntu20.04/images/sha256-7916b3cbe8b75c9bd324d3bc4a31eaa4b682202f255c4dda67abb99ebf79c13f?context=explore)
- 依存ライブラリ
  - OpenCV 4.5.4
  - Boost 1.71.0

### 2.2 環境構築方法

Dockerfileのあるルートディレクトリで以下のコマンドを実行し、Dockerイメージをビルドする。バージョン名は何でも問題ないが、使用するソフトのバージョンに応じて適宜変更することが望ましい。ここで指定した`dehazing-sample:v0.1`がタグを含んだDockerイメージ名となり、以降の操作で必要となる。

```shell
$ docker build -t dehazing-sample:v0.1 .
```

次に、下記のコマンドを実行してDockerコンテナを起動する。ここでは、現在のディレクトリをDockerコンテナ上の`/app/dehazing-sample`ディレクトリにマウントしている。マウント元ディレクトリやマウント先ディレクトリは適宜変更して良いが、本プログラムをマウントする必要がある点に注意。`--name`オプションでDockerコンテナ名を`dehazing-sample-v0.1`と指定している。

```shell
$ docker run --gpus all -itd -v $PWD:/app/dehazing-sample --name dehazing-sample-v0.1 dehazing-sample:v0.1
```

これ以降、プログラムのビルドや実行はDockerコンテナ上で行うが、その前に下記のコマンドを実行して起動済みのDockerコンテナに入る。


```shell
$ docker exec -it dehazing-sample-v0.1 bash
```

なお、`docker run`コマンドにより起動済みのDockerコンテナを開始/終了する際は下記のコマンドを実行する。

```shell
$ docker start dehazing-sample-v0.1
$ docker stop dehazing-sample-v0.1
```


## 3. プログラムのビルド

```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
```

なお、`tests`ディレクトリ以下にあるテストプログラムを実行する場合は、cmakeを実行する際`cmake ..`の代わりに`cmake -DBUILD_TESTS ..`を実行する。

## 4. プログラムの実行

プログラムをビルドすると、実行ファイル`build/samples/main_dehazing`が生成されるので、こちらを実行する。このとき、ディレクトリ構成は以下の通り仮定する。`dataset/input`ディレクトリには入力画像が格納されており、`dataset/output`ディレクトリは出力画像を生成するために用意した空のディレクトリである。

```
dehazing-sample
├── build
│   └── samples
│       └── main_dehazing
├── dataset
│   ├── input
│   │   ├── 0001.png
│   │   ├── 0002.png
│   │   └── ...
│   └── output
├── include
├── README.md
├── samples
├── src
└── tests
    └── data
        └── parameters.json
```

プログラムの実行には、第1引数には入力画像を配置したディレクトリを、第2引数には出力画像を生成する先のディレクトリを指定する。第3引数にはパラメータを設定したJSONファイルへのパスを指定する。なお、第2引数で指定する出力先ディレクトリが存在しない場合は自動でディレクトリが生成される。

```shell
$ cd build
$ ./samples/main_dehazing ../dataset/input ../dataset/output ../tests/data/parameters.json
```

なお、`parameters.json`は以下の形式でパラメータを指定する。指定例は[tests/data/parameters.json](tests/data/parameters.json)に配置している。

```json
{
    "candidateAreaRate": 0.001,
    "darkChannelNeighborRadius": 15,
    "minTransmission": 0.1,
    "omega": 0.95,
    "guidedFilterRadius": 30,
    "guidedFilterEps": 30
}
```

|キー                      |備考                                               |
|-------------------------|---------------------------------------------------|
|candidateAreaRate        |環境光推定時に探索する領域の割合                        |
|darkChannelNeighborRadius|Dark channel画像生成時のモルフォロジー演算の半径         |
|minTransmission          |透過率の下限値                                       |
|omega                    |透過率計算時、透過率が小さくなりすぎないようにする係数      |
|guidedFilterRadius       |透過率マップの平滑化に用いるGuided filterのフィルタサイズ |
|guidedFilterEps          |透過率マップの平滑化度合い                             |


[^he]: K. He, J. Sun, and X. Tang, “Single image haze removal using dark channel prior,” *IEEE Trans. Pattern Anal. Mach. Intell.,* vol. 33, no. 12, pp. 2341–2353, 2011.
