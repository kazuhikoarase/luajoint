# 概要 #

Lua Joint は 3DCGソフトウェア Shade のジョイントを連動させるプラグインです。
連動の設定を Lua 言語で記述します。
対応しているジョイントは以下のとおりです。

  * 直線移動
  * 回転
  * 拡大縮小
  * 均等拡大縮小
  * 光源
  * パス
  * 変形
  * カスタムジョイント

> <a href='http://www.youtube.com/watch?feature=player_embedded&v=Tz9r3ACcm6A' target='_blank'><img src='http://img.youtube.com/vi/Tz9r3ACcm6A/0.jpg' width='425' height=344 /></a>

# 使用方法 #

Shade のプラグインフォルダにプラグインをコピーします。

> lua\_joint.dll (Windows)

> lua\_joint.shdplugin (Macintosh)

連動させたいジョイントパートの名称の頭に「lua」と付けます。
任意で「:(コロン)」以降に Lua コードを記述します。
「lua」と「:」の間の文字は無視されるので、必要に応じて判別しやすい名称を指定します。

ジョイントを操作すると、子階層のジョイントのコードが順に実行されます。
ただし、操作したジョイント自体のコードは実行されません。

実行順序は通常、親から子へ、ブラウザの表示順と同じです。

> ![http://luajoint.googlecode.com/svn/assets/order1.jpg](http://luajoint.googlecode.com/svn/assets/order1.jpg)

親階層の名称の頭に「lua~(チルダ)」と付けると、
子階層のジョイントのコードが先に実行されます。
子階層の位置によってジョイント値を決定したい場合に使用します。

> ![http://luajoint.googlecode.com/svn/assets/order2.jpg](http://luajoint.googlecode.com/svn/assets/order2.jpg)

# 関数 #

以下の関数を使用してジョイント値を制御します。
この他に[数学関数](http://www.lua.org/manual/5.1/manual.html#5.6)が利用可能です。
<table border='1'>
<tr><td>
<b>get_v(n)</b> </td><td> n階層上のジョイントの値を取得します。<br>
<ul><li>引数:<br>
<ul><li>n - 階層インデックス(0は現在のジョイント)<br>
</li></ul></li><li>戻り値:<br>
<ul><li>ジョイント値<br>
</td></tr>
<tr><td>
<b>set_v(v)</b> </td><td> 現在のジョイントに値を設定します。<br>
</li></ul></li><li>引数:<br>
<ul><li>v - ジョイント値<br>
</li></ul></li><li>戻り値:<br>
<ul><li>無し<br>
</td></tr>
<tr><td>
<b>get_p(n)</b> </td><td> n階層上のジョイントの位置(グローバル座標系)を取得します。<br>
</li></ul></li><li>引数:<br>
<ul><li>n - 階層インデックス(0は現在のジョイント)<br>
</li></ul></li><li>戻り値:<br>
<ul><li>位置(グローバル座標系) キー x, y, z を含むテーブル。<br>
</td></tr>
<tr><td>
<b>print(msg)</b> </td><td> メッセージウィンドウにデバッグ用メッセージを出力します。<br>
</li></ul></li><li>引数:<br>
<ul><li>msg - メッセージ<br>
</li></ul></li><li>戻り値:<br>
<ul><li>無し</li></ul></li></ul>

※ テーブル (get_p の戻り値など) の表示には対応していません。 キー x, y, z を個別に表示するように記述します。<br>
<br>
※ レンダリング中は出力しません。<br>
</td></tr>
</table>

# [設定例](Sample.md) #

※ Shade は、株式会社イーフロンティアの商標です。