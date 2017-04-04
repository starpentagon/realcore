1. 棋譜データベース(20170326版)

  以下の３つの棋譜データベースをマージしたデータ
  - ORC Game Center(921,624局)
  - RenjuNet(44,829局)
  - Renjuoffline(107,334局)
  計: 1,073,787局

2. 正規性, 終端のチェック

  Renju record checkerで正規性(空点への着手、禁手チェック、四ノビの防手)、終端条件（達四、四々、極め手）をチェック
  - 正規性: 144,685局(13.5%)
  - 終端条件: 222,726局(20.7%)
  - 対局結果の修正: 10,754(1.0%)

Appendix
A1. ORC Game Centerデータの生成方法

  1. 各月のtar.gzのSGFファイルをRenjuSGFParserでCSVに変換
  2. CSVをマージ
  3. RenjuBaseのHPからダウンロードしたデータを追加
  4. 日付, 黒番の対局者名, 白番の対局者名で降順ソート
  5. 重複行を除去
  6. イベント名をnoneからorcに置換
  7. ヘッダを付与

A2. Renjunetデータの生成方法

  1. RenjunetのHPからダウンロード
  2. ダウンロードしたデータをRenjuBaseでSGF形式に変換
  3. RenjuSGFParserでCSVに変換

A3. Renjuofflineの生成方法

  1. RenjuofflineのHPからダウンロード
  2. ダウンロードしたデータをRenjuBaseでSGF形式に変換
  3. RenjuSGFParserでCSVに変換
