# FONTSAVE.X

Active font data saver for X680x0/Human68k

このツールは現在有効になっているフォントデータをファイルに書き出します。

X680x0/Human68k 上では HIOCS.X をはじめとして、カスタムフォントを利用することのできる様々なツール・ドライバがあります。
また、丸文字・漫画文字など多くのカスタムフォントデータが流通しました。

8x16文字はこのフォント、16x16の非漢字はこのフォント...と場合によっては複数のフォントドライバを組み合わせて使うこともできますが、煩雑です。
このツールを使えば一つのフォントファイルにまとめることができ、Human68k起動時にHIOCS.Xに/Fオプションを付けて渡すだけでokになります。

# Install

FNTSVxxx.ZIP をダウンロードして展開し、FONTSAVE.X をパスの通ったディレクトリにコピーします。

# Usage

    fontsave <mode> <output-file>

    mode ... 0:8x16 * 128 chars (2,048bytes)
             1:8x16 * 256 chars (4,096bytes)
             2:8x16 * 256 chars + 16x16 * non-kanji (49,216bytes)
             3:8x16 * 256 chars + 16x16 * non-kanji+1st level kanji (145,472bytes)
             4:8x16 * 256 chars + 16x16 * non-kanji+1st/2nd level kanji (286,848bytes)

Example:
    fontsave 2 HOGEHOGE.FON
    hiocs /f HOGEHOGE.FON
