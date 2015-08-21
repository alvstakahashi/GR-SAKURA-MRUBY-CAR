# GR-SAKURA-MRUBY-CAR
ルネサスナイト7 でLT で発表した、mrubyのリモコンカーです。
mrubyOSのAPIを使ったmrubyプログラムのコンパイルしたバイナリーをアップロードして動作させることができます。

tarosayさんのwakayama.rbボードでもそのまま動きます。
FDTツールでfirmwareディレクトリのmotファイルを書いて動きます。


2015.08.21 リリースノート　Ver0.002
wakayama.rb ボードでの動作確認をしました。
変更点
1.IO2のピン　IO5のピンがでていないので割り当て変更
　IO11の出力ポート設定の追加　mrbgems rx-gpio.cおよび　rb_main.rb
  UPLOADモード設定判断ポートの変更　IO5 --> IO10
2.バイナリファイルディレクトリを追加

以上

GR-SAKURA-MRUBY-CAR-20150709-release-note
1.機能追加
　IO5をGNDにつないで起動によりUPLOADモードをスキップするようにしました。


2.不具合修正
　Rx-gpio 入力がたぶん動かなかったので修正しました。未テストのままです



GR-SAKURA-MRUBY-CAR-20150707-03release-note
1.機能追加　ありません
2.不具合等　対応
 RXduino および特電ライブラリを削除
 使ったいないのに、入っていたので削除しました。
 コンパイラのライブラリ以外、全部ソースあります。

GR-SAKURA-MRUBY-CAR-20150707-02release-note
1.機能追加　ありません
2.不具合等　対応
　流用元を一部変更した。



過去のリリースノート

GR-SAKURA-MRUBY-CAR-20150707 リリースノート

1.機能追加
  mrubyバイナリのアップロード機能を追加
　アップロードしたmrubyを実行します。
　まずローディング用のプロンプトモードになります。
　ロードしない場合は内蔵のプログラムを実行します。
　RAMにしかロードしないので、再起動時は再アップロードが必要です。

　c:\>mrbc sample.rb
 のように　sample.mrb ファイルを作り、L コマンドで　ターミナルで
 転送して、E コマンドで、実行します。

2.不具合対応と機能追加
　mrbgems tx-gpio の処理が間違っていたので修正。
　対応ポートを増加。入力にも対応
　追加したポートおよび入力は未テスト

 "IO4"
 "IO5"

 "IO44"
 "IO45"
 "IO46"
 "IO47"
 "IO48"
 "IO49"
 "IO50"
 "IO51"

 "IO2IN"
 "IO3IN"
 "IO4IN"
 "IO5IN"

 "IO44IN"
 "IO45IN"
 "IO46IN"
 "IO47IN"
 "IO48IN"
 "IO49IN"
 "IO50IN"
 "IO51IN"

　メソッド　in

例
	io2 = Rx_gpio.new(Rx_gpio::IO2IN)

	io2status = io2.in()

過去のリリースノート
GR-SAKURA-MRUBY-CAR-20150706-04 リリースノート

GR-SAKURA　mrubyスマホリモコンカー　です。

環境
ライブラリのmrubyは、MSYS環境を使っています。
ディレクトリ　mr_ruby/mruby-1.1.0 で　make

コンパイラは　KPIT rx-elf-gcc v12

全体は　ルネサスHEWプロジェクトでビルドします。

ルネサスE1デバッガを利用しています。

SCI0のシリアルコンソールからのコマンドでラジコンカーが動作します。

ハードウェアとしては
ステアリングは、PC7 アクセルはIO2を、利用しています
