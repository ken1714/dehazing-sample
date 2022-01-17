# ヘイズ除去プログラム
## 1. 概要

静止画像を入力し、ヘイズを除去した画像を出力するプログラム。He[^he]の手法をベースに実装している。

## 2. 実行環境とその構築

Dockerコンテナ上での実行を想定している。以降、その構成と環境構築方法を述べる。

### 2.1 Docker環境の構成

- ベースイメージ: [nvidia/cuda:11.4.2-devel-ubuntu20.04](https://hub.docker.com/layers/nvidia/cuda/11.4.2-devel-ubuntu20.04/images/sha256-594f08b6eb93c59b190c92badc0d9204cc65b014de8923482ad93c21f64ae9ab?context=explore)
- 依存ライブラリ
  - OpenCV 4.5.4
  - Boost 1.71.0

### 2.2 環境構築方法

Dockerfileのあるディレクトリで以下のコマンドを実行し、Dockerイメージをビルドする。バージョン名は使用するソフトのバージョンに応じて適宜変更すること。

```shell
$ docker build -t dehazing-sample:v0.1 .
```

次に、下記のコマンドを実行してDockerコンテナを起動する。ここでは、現在のディレクトリをDockerコンテナ上の`/app/dehazing-sample`ディレクトリにマウントしている。マウント元ディレクトリやマウント先ディレクトリは適宜変更して良いが、本プログラムをマウントする必要がある点に注意。

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

プログラムをビルドすると、実行ファイル`build/samples/main_dehazing`が生成されるので、こちらを実行する。このとき、ディレクトリ構成は以下の通り仮定する。

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

プログラムの実行には、第1引数には入力画像を配置したディレクトリを、第2引数には出力画像を生成する先のディレクトリを指定する。第3引数にはパラメータを設定したJSONファイルへのパスを指定する。

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


## 参考文献

[^he]: K. He, J. Sun, and X. Tang, “Single image haze removal using dark channel prior,” *IEEE Trans. Pattern Anal. Mach. Intell.,* vol. 33, no. 12, pp. 2341–2353, 2011.
