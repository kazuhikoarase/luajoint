# Lua Joint 設定例 #



# #1 異なる周期の回転ジョイントを連動 #

lua ROOT(カスタムジョイント)を動かすと、全ての歯車が連動します。

![http://luajoint.googlecode.com/svn/assets/browser1.jpg](http://luajoint.googlecode.com/svn/assets/browser1.jpg)

>> [サンプルをダウンロード](http://code.google.com/p/luajoint/downloads/list)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=Tz9r3ACcm6A' target='_blank'><img src='http://img.youtube.com/vi/Tz9r3ACcm6A/0.jpg' width='425' height=344 /></a>

# #2 直線移動ジョイントと回転ジョイントの連動 #

lua ROOT(カスタムジョイント)を動かすと、歯車とラックが連動します。

![http://luajoint.googlecode.com/svn/assets/browser2.jpg](http://luajoint.googlecode.com/svn/assets/browser2.jpg)

>> [サンプルをダウンロード](http://code.google.com/p/luajoint/downloads/list)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=ay8_PbccLsk' target='_blank'><img src='http://img.youtube.com/vi/ay8_PbccLsk/0.jpg' width='425' height=344 /></a>

# #3 関数を利用した連動 #

lua ROOT(カスタムジョイント)を動かすと、全ての翼が連動します。
ダミーのカスタムジョイントに関数 fn を定義して
個々のジョイントからこれを呼び出すことにより、 動きを一元制御しています。
また、変数 d を使って翼の角度を少しずつずらすようにしています。

![http://luajoint.googlecode.com/svn/assets/browser3.jpg](http://luajoint.googlecode.com/svn/assets/browser3.jpg)

>> [サンプルをダウンロード](http://code.google.com/p/luajoint/downloads/list)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=5cNoJHgik4A' target='_blank'><img src='http://img.youtube.com/vi/5cNoJHgik4A/0.jpg' width='425' height=344 /></a>

# #4 子要素の位置情報を利用した連動 #

lua ROOT(カスタムジョイント)を動かすと、クランクが連動します。

子要素を部分的にテストする際に、関数が未定義でもエラーとならないよう、
定義済みフラグ flg で判断しています。

![http://luajoint.googlecode.com/svn/assets/browser4.jpg](http://luajoint.googlecode.com/svn/assets/browser4.jpg)

>> [サンプルをダウンロード](http://code.google.com/p/luajoint/downloads/list)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=EkolCZ7Ce5Q' target='_blank'><img src='http://img.youtube.com/vi/EkolCZ7Ce5Q/0.jpg' width='425' height=344 /></a>