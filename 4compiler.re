= ツールとコンパイラ

#@# Page77

#@# This chapter will cover a little introduction to existing tooling for making things easy when developing using ClojureScript. Unlike the previous chapter, this chapter intends to tell different stories each independent of the other.
本章では、ClojureScript を用いた開発を容易にするための既存のツールについて簡単に紹介します。前章とは異なり、本章の内容は、それぞれが独立した内容になっています。

== コンパイラの概要

#@# Getting Started with the Compiler

#@# At this point, you are surely very bored with the constant theoretical explanations about the language itself and will want to write and execute some code. The goal of this section is to provide a little practical introduction to the ClojureScript compiler.
前章の言語自体の理論的な説明にうんざりしてしまって、何かコードを書いて実行したいと思われているかもしれません。このセクションの目的は、ClojureScript コンパイラの実用的な紹介を少し行うことです。

#@# The ClojureScript compiler takes the source code that has been split over numerous directories and namespaces and compiles it down to JavaScript. Today, JavaScript hasa great number of different environments where it can be executed - each with its own peculiarities.
ClojureScript のコンパイラは、多くのディレクトリと名前空間で分けられたソースコードを取得して、JavaScript にコンパイルします。今日では JavaScript を実行できる環境は多くあり、それぞれに固有の特性があります。

#@# This chapter intends to explain how to use ClojureScript without any additional tooling. This will help you understand how the compiler works and how you can use it when other tooling is not available (such as link:http://leiningen.org/[leiningen] +link:https://github.com/emezeske/lein-cljsbuild[cljsbuild] orlink:http://boot-clj.com/[boot]).
この章では、ツールを追加せずに ClojureScript を使う方法を説明します。
これは、他のツール(Leiningen@<fn>{lein} や @<fn>{boot})が利用できない場合において、コンパイラの動作や利用法を理解するのに役立ちます。

//footnote[lein][http://leiningen.org]
//footnote[boot][http://boot-clj.com]

#@# Wiki@<fn>{id}
#@#//footnote[id][URL]

=== 実行環境

#@# Execution environments

#@# What is an execution environment? An execution environment is an engine where JavaScript can be executed. 9 For example, the most popular execution environment is abrowser (Chrome, Firefox, ...) followed by the second most popular -link:https://Node.js.org/[Node.js].
実行環境とは何でしょうか。実行環境は、JavaScript を実行できるエンジンです。例えば、最も一般的な実行環境はブラウザ(Chrome、Firefox等)であり、次に Node.js@<fn>{nodejs} が続きます。

//footnote[nodejs][https://Node.js.org]


#@# There are others, such as Rhino (JDK 6+), Nashorn (JDK 8+), QtQuick (QT),... butnone of them have significant differences from the first two. So,ClojureScript at the moment may compile code to run in the browser or in　Node.js-like environments out of the box.
他にも、Rhino(JDK6以降)、Nashorn(JDK8以降)、QtQuick(QT)等の実行環境がありますが、どれも最初の 2 つと大きな違いはありません。ですので、今のところ ClojureScript は、ブラウザや Node.js ですぐに実行できるコードにコンパイルするでしょう。

=== コンパイラのダウンロード

#@# Download the compiler

#@# Although the ClojureScript is self hosted, the best way to use it is just using the JVM implementation. To use it, you should have jdk8 installed. ClojureScript itself only requires JDK 7, but the standalone compiler that we are going to use in this chapter requires JDK 8, which can be found at http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
ClojureScript はセルフホストですが、ClojureScript を使用する最善の方法は JVM 実装を使用することです。これを使用するには、JDK8 がインストールされている必要があります。ClojureScript 自体に必要なのは JDK7 のみですが、この章で使用するスタンドアロンコンパイラには JDK8 が必要です。JDK8 は http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html にあります。


#@# Page78

#@# You can download the latest ClojureScript compiler using wget:
最新の ClojureScript コンパイラは、wget を使用してダウンロードできます。

//cmd{
wget https://github.com/clojure/clojurescript/releases/download/r1.9.36/cljs.jar
//}

#@# The ClojureScript compiler is packaged in a standalone executable jar file, so this is the only file (along with JDK 8) that you need to compile your ClojureScript source code to JavaScript.
ClojureScript コンパイラはスタンドアロンの実行可能な jar ファイルにパッケージされているため、ClojureScript のソースコードを JavaScript にコンパイルするために必要なファイルはこれと JDK8 のみです。


=== Node.js へのコンパイル

#@# Compile for Node.js

#@# Let’s start with a practical example compiling code that will target *Node.js*(hereafter simply "Node.js"). For this example, you should have Node.js installed.
まず、Node.js をターゲットにコードをコンパイルする例から始めましょう。この例では、Node.js をインストールする必要があります。

#@# There are different ways to install Node.js, but the recommended way is using nvm("Node.js Version Manager"). You can read the instructions on how to install and usenvm on its link:https://github.com/creationix/nvm[home page].
Node.js をインストールする方法はいくつかありますが、Node.js のバージョンマネージャーの nvm を使用することをお勧めします。インストール方法と nvm の使用法については、nvm のドキュメント@<fn>{nvm}を参照してください。

//footnote[nvm][https://github.com/creationix/nvm]

#@# When you have installed nvm, follow installing the latest version of Node.js:
nvm をインストールしたら、最新バージョンの Node.js をインストールします。

//cmd{
$ nvm install v6.2.0
$ nvm alias default v6.2.0
//}
    
#@# You can test if *Node.js* is installed in your system with this command:
次のコマンドで Node.js がインストールされているかをテストしましょう。

//cmd{
$ node --version
v6.2.0
//}


===== アプリケーションの作成

#@# Create the example application

#@# For the first step of our practical example, we will create our application directory structure and populate it with example code.
実用的な例を作成する最初のステップとして、アプリケーションのためのディレクトリを作成します。実用例に使うコードをそこに取り込んでいきます。

#@# Start by creating the directory tree structure for our “hello world” application:
まず、"Hello world" アプリケーションのためのディレクトリを作成しましょう。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//cmd{
$ mkdir -p myapp/src/myapp
$ touch myapp/src/myapp/core.cljs
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page79
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


#@# Resulting in this directory tree:
次のようなディレクトリ構造にします。

//cmd{
myapp
└── src
    └── myapp
        └── core.cljs
//}

#@# Second, write the example code into the previously created`myapp/src/myapp/core.cljs` file:
次に、前に作成した @<code>{myapp/src/myapp/core.cljs} に次のコードを書き込みます。

//emlist{
(ns myapp.core
  (:require [cljs.nodejs :as nodejs]))

(nodejs/enable-util-print!)

(defn -main
  [& args]
  (println "Hello world!"))

(set! *main-cli-fn* -main)
//}

#@# NOTE: It is very important that the declared namespace in the file exactly matchesthe directory structure. This is the way ClojureScript structures its source code.
ファイル内で宣言された名前空間が、ディレクトリの構造と正確に一致することが非常に重要です。これは、ClojureScript がソースコードを構築する方法です。


===== アプリケーションのコンパイル

#@# Compile the example application

#@# In order to compile that source code, we need a simple build script that tells theClojureScript compiler the source directory and the output file. ClojureScripthas a lot of other options, but at this moment we can ignore that.
このソース・コードをコンパイルするためには、ClojureScript コンパイラにソース・ディレクトリと出力ファイルを伝える単純なビルド・スクリプトが必要です。ClojureScript には他にも多くの選択肢がありますが、現時点では取り上げません。


#@# Let’s create the `myapp/build.clj` file with the following content:
@<code>{myapp/build.clj} を作成します。このファイルの内容は次のとおりです。

//emlist{
(require '[cljs.build.api :as b])

(b/build "src"
 {:main 'myapp.core
  :output-to "main.js"
  :output-dir "out"
  :target :nodejs
  :verbose true})
//}


#@# This is a brief explanation of the compiler options used in this example:
//noindent
ここでは、この例で使用されているコンパイラのオプションについて簡単に説明します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# * The `:output-to` parameter indicates to the compiler the destination of the compiled  code, in this case to the "main.js" file.
//noindent
- :output-to は、コンパイルされたコードの出力先を指定します。この場合 main.js です。

#@# * The `:main` property indicates to the compiler the namespace that will act as the entry  point of your application when it's executed.
//noindent
- @<code>{:main} は、実行時にエントリーポイントとして動作する名前空間を指定します。

#@# * The `:target` property indicates the platform where you want to execute the compiled  code. In this case, we are going to use *Node.js*. If you omit this  parameter, the source will be compiled to run in the browser environment.
//noindent
-  @<code>{:target} は、コンパイルされたコードを実行するプラットフォームを示します。この例では、Node.js を使用します。このパラメーターを省略した場合、ソースはコンパイルされてブラウザの環境で実行されます。

#@# Page80
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# To run the compilation, just execute the following command:
コンパイルを実行するには、次のコマンドを実行します。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//cmd{
$ cd myapp
$ java -cp ../cljs.jar:src clojure.main build.clj
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# And when it finishes, execute the compiled file using *node*:
コンパイル後、node コマンドでコンパイルされたファイルを実行します。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//cmd{
$ node main.js
Hello world!
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

=== ブラウザへのコンパイル

#@# Compile for the Browser

#@# In this section we are going to create an application similar to the "hello world"example from the previous section to run in the browser environment. The minimalrequirement for this application is just a browser that can execute JavaScript.
このセクションでは、前のセクションの "Hello world" の例と同様のアプリケーションを作成し、ブラウザの環境で実行します。このアプリケーションの最小要件は、JavaScript を実行できるブラウザだけです。

#@# The process is almost the same, and the directory structure is the same. The onlythings that changes is the entry point of the application and the build script. So,start re-creating the directory tree from previous example in a different directory.
プロセスはほぼ同じで、ディレクトリの構造も同じです。変更するのは、アプリケーションのエントリーポイントとビルド・スクリプトだけです。前の例のディレクトリ・ツリーを元に、別のディレクトリを再生成します。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//cmd{
$ mkdir -p mywebapp/src/mywebapp
$ touch mywebapp/src/mywebapp/core.cljs
//}

#@# Resulting in this directory tree:
結果、次のディレクトリ構造になります。

//cmd{
mywebapp
└── src
    └── mywebapp
        └── core.cljs
//}

#@# Then, write new content to the `mywebapp/src/mywebapp/core.cljs` file:
次に、新しいコンテンツを @<code>{mywebapp/src/mywebapp/core.cljs} に書き込みます。

//emlist{
(ns mywebapp.core)

(enable-console-print!)

(println "Hello world!")
//}

#@# In the browser environment we do not need a specific entry point for the application,so the entry point is the entire namespace.
ブラウザの環境では、アプリケーションのための特定のエントリーポイントは必要ないため、全体の名前空間がエントリーポイントになります。

@<embed>{|latex|\vspace{-0.5\Cvs\}}


#@# Page81
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

===== アプリケーションのコンパイル

#@# Compile the example application

#@# In order to compile the source code to run properly in a browser, overwrite the`mywebapp/build.clj` file with the following content:
ソースコードをコンパイルしてブラウザで正しく動作させるには、@<code>{mywebapp/build.clj} を次のように上書きします。

//emlist{
(require '[cljs.build.api :as b])

(b/build "src" {:output-to "main.js"
                :source-map true
                :output-dir "out/"
                :main 'mywebapp.core
                :verbose true
                :optimizations :none})
//}

#@# This is a brief explanation of the compiler options we're using:
//noindent
ここで使用するコンパイラオプションについて簡単に説明します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# * The `:output-to` parameter indicates to the compiler the destination of the  compiled code, in this case the "main.js" file.
//noindent
- @<code>{:output-to} は、コンパイルされたコードの出力先を指定します。この場合は main.js です。

#@# * The `:main` property indicates to the compiler the namespace that will act as  the  entry point of your application when it's executed.
//noindent
- @<code>{:main} は、実行時にエントリーポイントとして動作する名前空間を指定します。

#@# * `:source-map` indicates the destination of the source map. (The source map connects the ClojureScript source to the generated JavaScript so that error  messages can point you back to the original source.)
//noindent
- @<code>{:source-map} は、ソースマップの出力先を指定します。ソースマップは、ClojureScript のソースを、生成された JavaScript に接続して、エラー・メッセージが元のソースを示すようにします。

#@# * `:output-dir` indicates the destination directory for all file sources used in a  compilation. It is just for making source maps work properly with the rest of the  code, not only your source.
//noindent
- @<code>{:output-dir} は、コンパイルで使われる全てのソースを保存するディレクトリを指定します。ソースマップをソースだけでなく、残り全てのコードと正しく動作させるためのものです。

#@# `:optimizations` indicates the compilation optimization. There are different  values for this option, but that will be covered in subsequent sections in  more detail.
//noindent
- @<code>{:optimizations} はコンパイルの最適化を示します。このオプションには異なる値がありますが、これについては後のセクションで詳しく説明します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# To run the compilation, just execute the following command:
コンパイルを実行するには、次のコマンドを実行します。

//cmd{
cd mywebapp
java -cp ../cljs.jar:src clojure.main build.clj
//}

#@# This process can take some time, so do not worry; wait a little bit. The JVM bootstrap with the Clojure compiler is slightly slow. In the following sections, we will explain how to start a watch process to avoid constantly starting and stopping this slow process.
この処理には時間がかかることがありますが、心配せずに少し待ってください。Clojure コンパイラを使用した JVM のブートストラップは少し遅いです。以降のセクションでは、このような遅いプロセスの起動と停止を頻繁に行わないように、監視プロセスを開始する方法を説明します。

#@# While waiting for the compilation, let's create a dummy HTML file to make it easy toexecute our example app in the browser. Create the _index.html_ file with thefollowing content; it goes in the main _mywebapp_ directory.
コンパイルを待つ間に、ダミーの HTML ファイルを作成して、ブラウザでサンプル・アプリケーションを簡単に実行できるように、ダミーの HTML ファイルを作成しましょう。以下の内容を mywebapp ディレクトリの index.html に書き込みます。

#@# Page82
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
<!DOCTYPE html>
<html>
  <header>
    <meta charset="utf-8" />
    <title>Hello World from ClojureScript</title>
  </header>
  <body>
    <script src="main.js"></script>
  </body>
</html>
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# Now, when the compilation finishes and you have the basic HTML file you can just openit with your favorite browser and take a look in the development tools console. The"Hello world!" message should appear there.
コンパイルが完了して基本的な HTML ファイルができたら、好きなブラウザで開いて、開発ツールのコンソールで調べることができます。"Hello world!" というメッセージが表示されます。


=== 監視プロセス

#@# Watch process

#@# You may have already noticed the slow startup time of the ClojureScriptcompiler. To solve this, the ClojureScript standalone compiler comes with atool to watch for changes in your source code, and re-compile modified files assoon as they are written to disk.
ClojureScript コンパイラの起動時間が遅いことに既に気づいているかもしれません。この問題を解決するために、ClojureScript のコンパイラには、ソースコードの変更を監視して、ファイルの変更がディスクに書き込まれると同時に再コンパイルするツールが付属しています。

#@# Start by creating another build script, but this time name it watch.clj:
別のビルド・スクリプトを watch.clj という名前で作成します。

//emlist{
(require '[cljs.build.api :as b])

(b/watch "src"
 {:output-to "main.js"
  :output-dir "out/"
  :source-map true
  :main 'mywebapp.core
  :optimizations :none})
//}

#@# Now, execute the script just like you have in previous sections:
ここで、前のセクションと同じようにスクリプトを実行します。

//cmd{
$ java -cp ../cljs.jar:src clojure.main watch.clj
Building ...
Reading analysis cache for jar:file:/home/niwi/cljsbook/playground/cljs.jar!/cljs/core.cljs
Compiling src/mywebapp/core.cljs
Compiling out/cljs/core.cljs
Using cached cljs.core out/cljs/core.cljs
... done. Elapsed 0.754487937 seconds
Watching paths: /home/niwi/cljsbook/playground/mywebapp/src
//}

#@# Go back to the mywebapp.core namespace, and change the print text to "Hello World, Again!". You’ll see that the file src/mywebapp/core.cljs the file is immediately recompiled, and if you reload index.html in your browser the new text is displayed in the developer console. Another advantage of this method is that it gives a little bit more output.
名前空間を mywebapp.core に戻して、出力テキストを Hello World, Again! に変更すると、src/mywebapp/core.cljs がすぐにコンパイルされます。ブラウザで index.html をリロードすると、新しい出力がコンソールに表示されます。この方法の別の利点は、出力が少し増えることです。

#@# Page84−2
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


=== 最適化レベル

#@# Optimization levels

#@# The ClojureScript compiler has different levels of optimization. Behind the scenes,those compilation levels are coming from the Google Closure Compiler.
ClojureScript コンパイラにはさまざまな最適化レベルがあります。これらのコンパイルレベルは 舞台裏で行われますが、Google Closure Compiler によります。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# A simplified overview of the compilation process is:
コンパイルのプロセスは次の通りです。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# 1. The reader reads the code and does some analysis. This compiler may raise some   warnings during this phase.
//noindent
1. リーダーはコードを読み、何らかの分析を行います。この段階でコンパイラは警告を発する場合があります。

@<embed>{|latex|\allowbreak }

#@# 2. Then, the ClojureScript compiler emits JavaScript code. The result is one   JavaScript output file for each ClojureScript input file.
//noindent
2. その後、ClojureScript コンパイラは ClojureScript の 1 ファイルごとに 1 つのJavaScript の出力ファイルを作成します。

@<embed>{|latex|\allowbreak }

//noindent
3.生成された JavaScript のファイルは Google Closure コンパイラに渡され、最適化レベルや他のオプション(ソースマップや output デイレクトリ)に応じて最終的な出力ファイルが生成されます。

@<embed>{|latex|\allowbreak }

@<embed>{|latex|\vspace{2\Cvs\}}

#@# The final output format depends on the optimization level chosen:
最終的な出力形式は、選択した最適化レベルによって異なります。

@<embed>{|latex|\vspace{0.5\Cvs\}}


===== none

#@# This optimization level causes the generated JavaScript to be written into separateoutput files for each namespace, without any additional transformations to the code.
この最適化レベルでは、生成された JavaScript は名前空間ごとに別々の出力ファイルに書き込まれ、コードへの追加の変換はありません。

===== whitespace

#@# This optimization level causes the generated JavaScript files to be concatenated intoa single output file, in dependency order.  Line breaks and other whitespace areremoved.
この最適化レベルでは、生成された JavaScript ファイルが依存関係の順で出力ファイルに連結されます。改行やその他の空白は削除されます。

#@# This reduces compilation speed somewhat, resulting in a slower compilations. However,it is not terribly slow and it is quite usable for small-to-medium sized applications.

これによりコンパイル速度が多少低下し、コンパイルが遅くなります。ただし、処理速度はそれほど遅くはなく、中小規模のアプリケーションには十分に使用できます。

===== simple

#@# The simple compilation level builds on the work from the `whitespace` optimizationlevel, and additionally performs optimizations within expressions and functions, suchas renaming local variables and function parameters to have shorter names.
単純なコンパイルレベルは、@<code>{whitespace} 最適化レベルからの作業に基づいて構築されます。ローカル変数や関数パラメータの名前をより短い名前に変更するなど、式や関数内でさらに最適化を実行します。

#@# Compilation with the `:simple` optimization always preserves the functionality ofsyntactically valid JavaScript, so it does not interfere with the interaction betweenthe compiled ClojureScript and other JavaScript.
@<code>{:simple} 最適化によるコンパイルは、常に構文的に有効な JavaScript の機能を保持するため、コンパイルされた ClojureScript と他の JavaScript との間の相互作用に影響はありません。

===== advanced

#@# The advanced compilation level builds on the `simple` optimization level, andadditionally performs more aggressive optimizations and dead code elimination. Thisresults in a significantly smaller output file.
高度なコンパイルレベルは @<code>{simple} 最適化レベルの上に構築され、さらに積極的な最適化とデッドコードの削除を実行します。これにより、出力ファイルが大幅に小さくなります。

#@# The `:advanced` optimizations only work for a strict subset of JavaScript whichfollows the Google Closure Compiler rules.  ClojureScript generates validJavaScript within this strict subset, but if you are interacting with third partyJavaScript code, some additional work is required to make everything work asexpected.
@<code>{:advanced} 最適化は、Google Closure Compiler ルールに従った JavaScript の厳密なサブセットでのみ動作します。ClojureScript はこの厳密なサブセット内に有効な JavaScript を生成しますが、サード・パーティーの JavaScript コードを操作する場合には、全てを期待どおりに動作させるために追加作業が必要になります。

#@# This interaction with third party javascript libraries will be explained in later sections.
サードパーティの JavaScript ライブラリとのやり取りについては、後のセクションで説明します。


#@# Page85
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


== REPLでの作業

#@# Working with the REPL

=== 導入

#@# Introduction

#@# Although you can create a source file and compile it every time you want to trysomething out in ClojureScript, it's easier to use the REPL. REPL stands for:

#@# In other words, the REPL lets you try out ClojureScript concepts and get immediate feedback.
ソースファイルを作成して、ClojureScript で何かを試したいときに毎回コンパイルすることもできますが、REPL を使用するのが簡単です。REPL とは、Read(キーボードかた入力を読みとり)、Evaluate(入力を評価して)、Print(結果を出力する)、Loop(入力に戻り繰り返す)の頭文字をとったものです。言い換えると、REPL によって ClojureScript のコンセプトを試して、即座にフィードバックを得ることができます。

#@# ClojureScript comes with support for executing the REPL in different executionenvironments, each of which has its own advantages and disadvantages. For example,you can run a REPL in Node.js but in that environment you don't have any access to theDOM.  Which REPL environment is best for you depends on your specific needs andrequirements.
ClojureScript には、様々な実行環境で REPL を実行するためのサポートが用意されていますが、それぞれに長所と短所があります。たとえば、Node.js では REPL を実行できますが DOM にアクセスできません。どの REPL 環境が最適かは、個々のニーズと要件によって異なります。

=== Nashorn REPL

#@# The Nashorn REPL is the easiest and perhaps most painless REPL environment because itdoes not require any special stuff, just the JVM (JDK 8) that you have used in previous examples for running the ClojureScript compiler.
Nashorn REPLは、特別なものを必要とせず、ClojureScript コンパイラを実行するために、以前の例で使用したJVM(JDK8)のみを必要とするため、最も簡単で、おそらく最も苦痛のない REPL 環境です

#@# Let’s start creating the repl.clj file with the following content:
REPL の Playground のために新規でスクリプトファイル repl.clj を作成して、次のように編集します。

//emlist{
(require '[cljs.repl]
         '[cljs.repl.nashorn])

(cljs.repl/repl
 (cljs.repl.nashorn/repl-env)
 :output-dir "out"
 :cache-analysis true)
//}

#@# Then, execute the following command to get the REPL up and running:
次のコマンドにより REPL を起動して実行します。

//cmd{
$ java -cp cljs.jar:src clojure.main repl.clj
To quit, type: :cljs/quit
cljs.user=> (+ 1 2)
3
//}

#@# You may have noticed that the REPL does not have support for history and other shell-like facilities. This is because the default REPL does not comes with "readline" support. But this problem can be solved using a simple tool named rlwrap which you should be able to find find with the package manager of your operating system (e.g. for Ubuntu, type sudo apt install -y rlwrap to install).
お気付きかもしれませんが、REPL には、ヒストリーやその他のシェルのような機能がサポートされていません。これは、デフォルトの REPL では readline がサポートされていないためです。この問題は rlwrap という名前の簡単なツールを使えば解決できます。このツールはオペレーティングシステムのパッケージマネージャで見つけることができるはずです(Ubuntu の場合、sudo apt install -y rlwrap でインストールします)。

#@# Page86
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# The rlwrap tool gives the REPL "readline" capability, and will allow you to have command history, code navigation, and other shell-like utilities that will make your REPL experience much more pleasant. To use it, just prepend it to the previous command that we used to start the REPL:
rlwrap により REPL で readline が機能して、コマンドの履歴やコードナビゲーションなどのシェルのようなユーティリティが使うことができ、REPL をより快適に使うことができます。これを使用するには、REPL を開始するためには、使用した前のコマンドの前に rlwrap を追加します。

//cmd{
$ rlwrap java -cp cljs.jar:src clojure.main repl.clj
To quit, type: :cljs/quit
cljs.user=> (+ 1 2)
3
//}

=== Node.js REPL

#@# You must, of course, have Node.js installed on your system to use this REPL.
まず初めに Node.js が必要です。このREPLを使用するには、Node.js をシステムにインストールしてください。

#@# You may be wondering why we might want a Node.js REPL, when we already have thenashorn REPL available which doesn't have any external dependencies. The answer isvery simple: Node.js is the most used JavaScript execution environment on the backend,and it has a great number of community packages built around it.
外部依存関係を持たない Nashorn REPL がすでに使用可能になっているのに、なぜ Node.js が必要なのか不思議に思うかもしれません。。答えは非常に単純です。Node.js はバックエンドで最もよく使われている JavaScript 実行環境であり、その上に構築された数多くのコミュニティーによるパッケージが存在するからです。

#@# The good news is that starting a Node.js REPL is very easy once you have it installedin your system. Start adding the following content into `tools.clj` script:
幸いなことに、Node.js の REPLは、一度システムにインストールすれば非常に簡単に起動できます。次の内容を @<code>{tools.clj} に追加します。

//emlist{
(require '[cljs.repl]
         '[cljs.repl.node])

(cljs.repl/repl
 (cljs.repl.node/repl-env)
 :output-dir "out"
 :cache-analysis true)
//}
 
#@# And start the REPL like you have done it previously with nashorn REPL:
以前 Nashorn REPL で行ったように REPL を開始します。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//cmd{
$ rlwrap java -cp cljs.jar:src clojure.main repl.clj
To quit, type: :cljs/quit
cljs.user=> (+ 1 2)
3
//}


#@# Page87
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== Browser REPL

#@# This REPL is the most laborious to get up and running. This is because it uses abrowser for its execution environment and it has additional requirements.
この REPL は、起動と実行に最も手間がかかります。これは 実行環境 のためにブラウザを使用しており、追加の要件があるためです。

#@# Let’s start by creating a file named brepl.clj with the following content:
まず、brepl.clj という名前のファイルを作成して、次のように編集します。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(require
  '[cljs.build.api :as b]
  '[cljs.repl :as repl]
  '[cljs.repl.browser :as browser])

(b/build "src"
 {:output-to "main.js"
  :output-dir "out/"
  :source-map true
  :main 'myapp.core
  :verbose true
  :optimizations :none})

(repl/repl (browser/repl-env)
  :output-dir "out")
//}

 
#@# This script builds the source, just as we did earlier, and then starts the REPL.
このスクリプトは、先ほどと同じようにソースをビルドして REPL を開始します。

#@# But the browser REPL also requires that some code be executed in the browser before the REPL gets working. To do that, just re-create the application structure very similar to the one that we have used in previous sections:
ブラウザの REPL は、REPL が動作する前にブラウザ内でコードを実行することも要求します。そのために、前のセクションで使用したものとよく似たアプリケーションの構造を再作成します。

//cmd{
$ mkdir -p src/myapp
$ touch src/myapp/core.cljs
//}

#@# Then, write new content to the src/myapp/core.cljs file:
次に、新しいコンテンツを src/myapp/core.cljs に書き込みます。

//emlist{
(ns myapp.core
 (:require [clojure.browser.repl :as repl]))

(defonce conn
  (repl/connect "http://localhost:9000/repl"))

(enable-console-print!)

(println "Hello, world!")
//}

#@# And finally, create the missing index.html file that is going to be used as the entry point for running the browser side code of the REPL:
最後に、ブラウザ側のコードを実行するためのエントリーポイントとして使用する index.html を作成します。

#@# Page89
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

//emlist{
<!DOCTYPE html>
<html>
  <header>
    <meta charset="utf-8" />
    <title>Hello World from ClojureScript</title>
  </header>
  <body>
    <script src="main.js"></script>
  </body>
</html>
//}

#@# Well, that was a lot of setup! But trust us, it’s all worth it when you see it in action. To do that, just execute the brepl.clj in the same way that we have done it in previous examples:
多くの設定をしましたが、実際に動いているところを見てみると、それだけの価値があることは間違いありません。前例と同じ方法で brepl.clj を実行します。

//cmd{
$ rlwrap java -cp cljs.jar:src clojure.main brepl.clj
Compiling client js ...
Waiting for browser to connect ...
//}

#@# And finally, open your favourite browser and go to http://localhost:9000/. Once the page is loaded (the page will be blank), switch back to the console where you have run the REPL and you will see that it is up and running:
最後に、お気に入りのブラウザを開き、http://localhost:9000/. にアクセスします。ページがロードされるた後(ページは空白です)、REPL を実行したコンソールに戻り、REPL が実行されていることを確認しましょう。

//cmd{
[...]
To quit, type: :cljs/quit
cljs.user=> (+ 14 28)
42
//}

#@# One of the big advantages of the browser REPL is that you have access to everything in the browser environment. For example, type (js/alert "hello world") in the REPL. This will cause the browser to display an alert box. Nice!
ブラウザ REPL の大きな利点の 1 つは、ブラウザ環境のすべてにアクセスできることです。たとえば、REPL に (js/alert "hello world") と入力すると、ブラウザに警告ボックスが表示されます。

#@# Page90
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


== Google Closure Library

#@# The Closure Library

#@# The Google Closure Library is a javascript library developed by Google. It has amodular architecture, and provides cross-browser functions for DOM manipulations and events, ajax and JSON, and other features.
Google Closure Library は Google が開発している JavaScript ライブラリです。モジュールのアーキテクチャを備えており、DOM 操作、イベント処理、Ajax 、JSON を扱うためにクロスブラウザの関数を提供します。

#@# The Google Closure Library is written specifically to take advantage of the ClosureCompiler (which is used internally by the ClojureScript compiler).
Google Closure Library は、ClojureScript コンパイラで内部的に使用されている Closure Compiler を利用するために書かれています。

#@# ClojureScript is built on the Google Closure Compiler and Closure Library. In fact,ClojureScript namespaces are Closure modules. This means that you can interact withthe Closure Library very easily:
ClojureScript は Google Closure コンパイラと Closure Library の上に構築されています。実際、ClojureScript の名前空間は Closure モジュールです。これは、Closure Library を非常に簡単に操作できることを意味します。

//emlist{
(ns yourapp.core
  (:require [goog.dom :as dom]))

(def element (dom/getElement "body"))
//}

#@# This code snippet shows how you can import the *dom* module of the Closure libraryand use a function declared in that module.
このコードは、Closure Library の dom モジュールをインポートして、そのモジュールで宣言された関数を使用する方法を示しています。

#@# Additionally, the closure library exposes "special" modules that behave like a classor object. To use these features, you must use the `:import` directive in the `(ns...)` form:
さらに、Closure Library は、クラスまたはオブジェクトのように動作する「特別な」モジュールを公開します。これらの機能を使用するには、@<code>{(ns ...)} の形式で @<code>{:import} ディレクティブを使用する必要があります。

//emlist{
(ns yourapp.core
  (:import goog.History))

(def instance (History.))
//}

#@# In a _Clojure_ program, the `:import` directive is used for host (Java) interop toimport Java classes.  If, however, you define types (classes) in ClojureScript, youshould use the standard `:require` directive and not the `:import` directive.
Clojure のプログラムでは、Java クラスをインポートするためにホスト環境である Java と相互運用するために、@<code>{:import} ディレクティブを使用します。ただし、ClojureScript で型(クラス)を定義する場合は、@<code>{:import} ディレクティブではなく、標準の @<code>{:require} ディレクティブを使用する必要があります。


== 依存関係の管理

#@# Dependency management

#@# Until now, we have used the builtin _Clojure(Script)_ toolchain to compile oursource files to JavaScript. Now this is a time to understand how manage externaland/or third party dependencies.
ここまでは、組み込みの ClojureScript のツールを使ってソース・ファイルを JavaScript にコンパイルしてきました。次は、外部またはサード・パーティ(あるいはその両方)の依存関係を管理する方法について説明します。

#@# Page91
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

#@# For this reason, the remainder of this chapter will explain how to use Leiningen, the de facto clojure build and dependency management tool, for building ClojureScript projects. The boot build tool is also growing in popularity, but for the purposes of this book we will limit ourselves to Leiningen.
このため、この章の残りの部分では、ClojureScript プロジェクトを構築するために、事実上ビルドと依存関係管理ツールのデファクトである Leiningen の使い方を説明します。boot の人気も高まっていますが、本書では Leiningen に限定します。

=== Leiningen のインストール

#@# Installing leiningen

#@# The installation process of leiningen is quite simple; just follow these steps:
Leiningen のインストール方法は非常に簡単です。次の手順に従ってください。

//cmd{
$ mkdir ~/bin
$ cd ~/bin
$ wget https://raw.githubusercontent.com/technomancy/leiningen/stable/bin/lein
$ chmod a+x ./lein
$ export PATH=$PATH:~/bin
//}

#@# Make sure that the ~/bin directory is always set on your path. To make it permanent, add the line starting with export to your ~/.bashrc file (assuming you are using the bash shell).
~/bin ディレクトリが常にパスに設定されていることを確認します。永続的にするには、export で始まる行を ~/.bashrc に追加します。(bash を使用していると仮定します)。

#@# Now, open another clean terminal and execute lein version. You should see something like the following:
ターミナルを別で開いて lein version を実行します。次のように表示されます。

//cmd{
$ lein version
Leiningen 2.5.1 on Java 1.8.0_45 OpenJDK 64-Bit Server VM
//}

#@# We assume here that you are using a Unix-like system such as Linux or BSD. If you are a Windows user, please check the instructions on the Leiningen homepage. You can also get the Linux/Mac OS X/BSD version of the leiningen script at the web site.
ここでは、Linux や BSD のような Unix ライクなシステムを使っていると仮定しています。Windows ユーザの方は、Leiningen のホームページの説明を参照してください。Linux、Mac OS X、BSD 版の Leiningen スクリプトは Web サイトから入手することもできます。


=== 初めてのプロジェクト

#@# First project

#@# The best way to show how a tool works is by creating a toy project with it. Inthis case, we will create a small application that determines if a year is aleap year or not.
ツールの動作を紹介する最良の方法は、ツールを使用して toy プロジェクトを作成することです。この例では、閏(うるう)年かどうかを判別する小さなアプリケーションを作成します。

#@# To start, we will use the mies leiningen template.
まず、Leiningen の mies テンプレートを使用します。

#@# Templates are a facility in leiningen for creating an initial project structure. The clojure community has a great many of them. In this case we’ll use the mies template that was started by the clojurescript core developer. Consult the leiningen docs to learn more about templates.
テンプレートは、初めのプロジェクト構造を作成するための Leiningen の機能です。Clojure コミュニティには非常に多くのものがあります。この例では、ClojureScript の中心的な開発者が作成した mies テンプレートを使用します。テンプレートの詳細については、Leiningen のドキュメントを参照してください。

#@# Let’s start creating the project layout:
プロジェクトのレイアウトを作成します。

//cmd{
$ lein new mies leapyears
$ cd leapyears # 生成したプロジェクトのディレクトリに移動する
//}

#@# Page92
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

#@# The project has the following structure:
プロジェクトの構造は次のとおりです。

@<embed>{|latex|\vspace{-0.4\Cvs\}}


//emlist{
leapyears
├── index.html
├── project.clj
├── README.md
├── scripts
│   ├── build
│   ├── release
│   ├── watch
│   ├── repl
│   └── brepl
└── src
    └── leapyears
        └── core.cljs
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# The project.clj file contains information that Leiningen uses to download dependencies and build the project. For now, just trust that everything in that file is exactly as it should be.
project.clj には、Leiningen が依存関係をダウンロードしてプロジェクトをビルドするために使用する情報が含まれています。今のところ、project.clj はこういうものだと思ってください。

#@# Open the index.html file and add the following content at the beginning of body:
index.htmlファイルを開き、body の先頭に次の内容を追加します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
<section class="viewport">
  <div id="result">
    ----
  </div>
  <form action="" method="">
    <label for="year">Enter a year</label>
    <input id="year" name="year" />
  </form>
</section>
//}

#@# The next step is adding some code to make the form interactive. Put the following code into the src/leapyears/core.cljs:
次の手順では、フォームをインタラクティブにするコードを追加します。次のコードを src/leapyears/core.cljs に追加します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(ns leapyears.core
  (:require [goog.dom :as dom]
            [goog.events :as events]
            [cljs.reader :refer (read-string)]))

(enable-console-print!)

(def input (dom/getElement "year"))
(def result (dom/getElement "result"))

(defn leap?
  [year]
  (or (zero? (js-mod year 400))
      (and (pos? (js-mod year 100))
           (zero? (js-mod year 4)))))
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page93
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(defn on-change
  [event]
  (let [target (.-target event)
        value (read-string (.-value target))]
    (if (leap? value)
      (set! (.-innerHTML result) "YES")
      (set! (.-innerHTML result) "NO"))))

(events/listen input "keyup" on-change)
//}

#@# Now, compile the clojurescript code with:
次のようにして、ClojureScript コードをコンパイルします。

//cmd{
$ ./scripts/watch
//}

#@# Behind the scenes, the watch script uses the lein build tool to execute a command similar to the java build command from the previous sections:
バックグラウンドでは、監視スクリプトは lein ビルドツールを使用して、前のセクションで説明した Java の build コマンドに似たコマンドを実行します。

//cmd{
rlwrap lein trampoline run -m clojure.main scripts/watch.clj
//}

#@# You must have rlwrap installed on your system.
システムに rlwrap がインストールされている必要があります。

#@# Finally, open the index.html file in a browser. Typing a year in the textbox should display an indication of its leap year status.
最後に、ブラウザで index.html を開きます。テキストボックスに年を入力すると、うるう年の状態が表示されます。

#@# You may have noticed other files in the scripts directory, like build and release. These are the same build scripts mentioned in the previous section, but we will stick with watch here.
scripts ディレクトリには、build や release などのファイルがあります。これらは、前のセクションで言及したビルド・スクリプトと同じですが、ここでは watch を使います。

#@# Page94
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


=== 依存関係の管理

#@# Managing dependencies

#@# The real purpose of using Leiningen for the ClojureScript compilation process is to automate the retrieval of dependencies. This is dramatically simpler than retrieving them manually.

ClojureScript のコンパイルプロセスに Leiningen を使用する本当の目的は、依存関係の取得を自動化することです。これは、手動で取得するよりも大幅に簡単です。

#@# The dependencies, among other parameters, are declared in the project.clj file and have this form (from the mies template):

依存関係は、ほかのパラメータと一緒に project.clj で宣言されます。このファイルの形式は次の通りです(miesテンプレートより)。

//emlist{
(defproject leapyears "0.1.0-SNAPSHOT"
  :description "FIXME: write this!"
  :url "http://example.com/FIXME"
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [org.clojure/clojurescript "1.9.36"]
                 [org.clojure/data.json "0.2.6"]]
  :jvm-opts ^:replace ["-Xmx1g" "-server"]
  :node-dependencies [[source-map-support "0.3.2"]]
  :plugins [[lein-npm "0.5.0"]]
  :source-paths ["src" "target/classes"]
  :clean-targets ["out" "release"]
  :target-path "target")
//}

#@# And here is a brief explanation of the properties relevant for ClojureScript:
ClojureScript に関連するプロパティについて簡単に説明します。

#@# :dependencies: a vector of dependencies that your project needs.
:dependencies は、プロジェクトに必要な依存関係を含むベクタです。

#@# :clean-targets: a vector of paths that lein clean should delete.
:clean-targets は、lein clean により削除されるパスを含むベクタです。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# The dependencies in ClojureScript are packaged using jar files. If you are coming from Clojure or any JVM language, jar files will be very familiar to you. But if you aren’t familiar with them, do not worry: a .jar file is like a plain zip file that contains the project.clj for the library, some metadata, and the ClojureScript sources. The packaging will be explained in another section.
ClojureScript の依存関係は jar ファイルを使ってパッケージ化されます。Clojure やその他の JVM 言語を使用している場合、jar ファイルは非常になじみ深いものです。しかし、それらに慣れていなくても心配する必要はありません。jarファイルは、プロジェクトを含む単純な zip ファイルのようなものであり、ライブラリ用の project.clj、メタデータの一部、および ClojureScript のソースを含みます。パッケージングについては別のセクションで説明します。

#@# Clojure packages are often published on Clojars. You can also find many third party libraries on the ClojureScript Wiki.
Clojure パッケージは Clojars で公開されることが多いです。また、ClojureScript Wiki には多くのサードパーティのライブラリがあります。

@<embed>{|latex|\vspace{0.5\Cvs\}}

=== 外部の依存関係

#@# External dependencies

#@# In some circumstances you may found yourself that you need some library but thatdoes not exists in ClojureScript but it is already implemented in javascriptand you want to use it on your project.
ClojureScript には存在しませんが、すでに JavaScript に実装されていて、プロジェクトで使いたいライブラリがある場合があります。

#@# There are many ways that you can do it mainly depending on the library that you want to include. Let see some ways.
含めるライブラリによって、多くの方法があります。いくつか見ていきましょう。

#@# Page95
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


==== Closure 対応のライブラリ

#@# Closure Module compatible library

#@# If you have a library that is just written to be compatible with google closure module system and you want to include it on your project you should just put it in the source (classpath) and access it like any other clojure namespace.
Google Closure モジュールとの互換性を持つように作成されたライブラリをプロジェクトに含める場合には、そのライブラリをソース(クラスパス)に配置して、他の Clojure 名前空間と同じようにアクセスします。

#@# This is the most simplest case, because google closure modules are directly compatible and you can mix your clojure code with javascript code written using google closure module system without any additional steps.
これは最も単純なケースです。Google Closure モジュールは直接互換性があり、Google Closure モジュールシステムを使って書かれた JavaScript コードは、あなたの Clojure のコードと、追加の手順なしでミックスできます。

#@# Let play with it creating new project using mies template:
mies テンプレートを使用して、新しいプロジェクトを作成してみましょう。

//cmd{
$ lein new mies myextmods
$ cd myextmods
//}

#@# Create a simple google closure module for experiment:
実験用に簡単な Google Closure モジュールを作成します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
src/myextmods/myclosuremodule.js

@<embed>{|latex|\vspace{-1\Cvs\}}

//emlist{
goog.provide("myextmods.myclosuremodule");

goog.scope(function() {
  var module = myextmods.myclosuremodule;
  module.get_greetings = function() {
    return "Hello from google closure module.";
  };
});
//}

#@# Now, open the repl, require the namespace and try to use the exposed function:
REPL を開いて名前空間を reqire して、公開された関数を使用してみます。

you can open the nodejs repl just executing ./scripts/repl on the root of the repository.
レポジトリのルートで /scripts/repl を実行するだけで、Node.js の REPL を開くことができます。

//emlist{
(require '[myextmods.myclosuremodule :as cm])
(cm/get_greetings)
;; => "Hello from google closure module."
//}


==== CommonJS モジュール互換のライブラリ

#@# CommonJS modules compatible libraries

#@# Due to the Node.JS popularity the commonjs used in node is today the most used module format for javascript libraries, independently if they will be used in server side development using nodejs or using browser side applications.
Node.js  の人気により、Node.js で用いられる CommonJS の人気は、現在 JavaScript のライブラリで最も使用されているモジュール形式です。サーバサイド開発で使用されるか、ブラウザ側のアプリケーションで使用されるかにより、利用法は別々です。

#@# Let’s play with that. Start creating a simple file using commonjs module format (pretty analogous to the previous example using google closure modules):

#@# Page96
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

CommonJS のモジュール形式(Google Closure モジュールを用いた前例と類似点が多い)を使用した単純なファイルを作成します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
src/myextmods/mycommonjsmodule.js

@<embed>{|latex|\vspace{-1\Cvs\}}

//emlist{
function getGreetings() {
  return "Hello from commonjs module.";
}

exports.getGreetings = getGreetings;
//}

#@# Later, in order to use that simple pet library you should indicate to the ClojureScript compiler the path to that file and the used module type with :foreign-libs attribute.
後に、このシンプルな pet ライブラリを使うために、ClojureScript コンパイラにファイルのパスと、使用するモジュールの型を :foreign-libs 属性で指定する必要があります。

#@# Open scripts/repl.clj and modify it to somethig like this:
scripts/repl.clj を開いて、次のように変更します。

//emlist{
(require
  '[cljs.repl :as repl]
  '[cljs.repl.node :as node])

(repl/repl
 (node/repl-env)
 :language-in  :ecmascript5
 :language-out :ecmascript5
 :foreign-libs [{:file "myextmods/mycommonjsmodule.js"
                 :provides ["myextmods.mycommonjsmodule"]
                 :module-type :commonjs}]
 :output-dir "out"
 :cache-analysis false)
//}

#@# Although the direct path is used to point to this pet library you can specify a full URI to remote resource and it will be automatically downloaded.
直接的なパスはこの pet ライブラリを指すために使用されますが、リモートリソースへの完全な URI を指定することができ、自動的にダウンロードされます。

#@# Now, let’s try to play with moment within the repl (executing the ./scripts/repl script that uses the previously modified ./scripts/repl.clj file):
ここで、./scripts/repl.clj を用いた ./scripts/repl を実行して、REPL 内で moment を使ってみます。

//emlist{
(require '[myextmods.mycommonjsmodule :as cm])
(cm/getGreetings)
;; => "Hello from commonjs module."
//}

==== 従来のモジュール化されていない(グローバルスコープの)ライブラリ

#@# Legacy, module-less (global scope) libraries

#@# Although today is very common have libraries packaged using some kind of modules, there are also a great amount of libraries that just exposes a global objects and does not uses any kind of modules; and you may want to use them from ClojureScript.
今日では、何らかのモジュールを使用してライブラリをパッケージ化することが非常に一般的になっていますが、グローバルオブジェクトを公開するだけで、モジュールを全く使用しないライブラリも数多くありますが、ClojureScript から使うことができます。


#@# Page97
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# In order to use a library that exposes a global object, you should follow similar steps as with commojs modules with the exception that you should omit the :module-type attribute.
グローバルオブジェクトを公開するライブラリを使用するには、CommoJS モジュールと同様の手順を実行する必要がありますが、:module-type 属性を省略する点が異なります。

#@# This will create a synthetic namespace that you should require in order to be able to access to the global object through the js/ namespace. The namespace is called synthetic because it does not expose any object behind it, it just indicates to the compiler that you want that dependency.
これにより、名前空間の js/ を介してグローバルオブジェクトにアクセスするために必要な名前空間の合成が作成されます。名前空間は、その背後にあるオブジェクトを公開せず、その依存関係が必要であることをコンパイラに示すだけなので、synthetic と呼ばれます。

#@# Let’s play with that. Start creating a simple file declaring just a global function:
グローバル関数のみを宣言する単純なファイルを作成します。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
src/myextmods/myglobalmodule.js

@<embed>{|latex|\vspace{-1\Cvs\}}

//emlist{
function getGreetings() {
  return "Hello from global scope.";
}
//}

#@# Open scripts/repl.clj and modify it to somethig like this:
scripts/repl.clj を開いて、次のように変更します。

//emlist{
(require
  '[cljs.repl :as repl]
  '[cljs.repl.node :as node])

(repl/repl
 (node/repl-env)
 :language-in  :ecmascript5
 :language-out :ecmascript5
 :foreign-libs [{:file "myextmods/mycommonjsmodule.js"
                 :provides ["myextmods.mycommonjsmodule"]
                 :module-type :commonjs}
                {:file "myextmods/myglobalmodule.js"
                 :provides ["myextmods.myglobalmodule"]}]
 :output-dir "out"
 :cache-analysis false)
//}

#@# And in the same way as in previous examples, let evaluate that in the repl:
前の例と同様に、REPL 内で評価してみましょう。

//emlist{
(require 'myextmods.myglobalmodule)
(js/getGreetings)
;; => "Hello from global scope."
//}

#@# Page98
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


== Unit テスト

#@# Unit testing

#@# As you might expect, testing in ClojureScript consists of the same concepts widely used by other language such as Clojure, Java, Python, JavaScript, etc.
ご想像のとおり、ClojureScript でのテストは、Clojure、Java、Python、JavaScript などの他の言語で広く使用されているものと同じ概念で構成されています。

#@# Regardless of the language, the main objective of unit testing is to run some test cases, verifying that the code under test behaves as expected and returns without raising unexpected exceptions.
言語に関係なく、Unit テストの主な目的は、いくつかのテスト・ケースを実行し、テスト対象のコードが予期したとおりに動作し、予期しない例外を発生させずに戻ることを検証することです。

#@# The immutablity of ClojureScript data structures helps to make programs less error prone, and facilitates testing a little bit. Another advantage of ClojureScript is that it tends to use plain data instead of complex objects. Building "mock" objects for testing is thus greatly simplified.
ClojureScriptのデータ構造はイミュータブルであるため、プログラムのエラーが発生しにくく、テストが少し容易になります。ClojureScript のもう 1 つの利点は、複雑なオブジェクトではなく、プレーン・データを使用する傾向があることです。したがって、テスト用の「モック」オブジェクトの構築が大幅に簡略化されます。


=== 初めの一歩

#@# First steps

#@# The "official" ClojureScript testing framework is in the "cljs.test" namespace. It is a very simple library, but it should be more than enough for our purposes.
公式の ClojureScript のテスト・フレームワークは、名前空間 cljs.test にあります。これは非常に単純なライブラリですが、私たちの目的には十分すぎるはずです。

#@# There are other libraries that offer additional features or directly different approaches to testing, such as test.check. However, we will not cover them here.
他にも、追加の機能を提供したり、テストに対して直接的に異なるアプローチを提供するライブラリがあります (例えば test.check など)。ただし、ここでは説明しません。

#@# Start creating a new project using the mies leiningen template for experimenting with tests:
テストを試すために mies テンプレートを使用して、新しいプロジェクトを作成します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//cmd{
$ lein new mies mytestingapp
$ cd mytestingapp
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# This project will contain the same layout as we have seen in the dependency management subchapter, so we won’t explain it again.
#@# The next step is a creating a directory tree for our tests:

このプロジェクトには、依存関係管理のセクションで説明したのと同じレイアウトなので、ここでは説明しません。
次に、テスト用のディレクトリツリーを作成します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//cmd{
$ mkdir -p test/mytestingapp
$ touch test/mytestingapp/core_tests.cljs
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Also, we should adapt the existing watch.clj script to work with this newly created test directory:
また、この新しく作成されたテストディレクトリで動作するように、既存の watch.clj を改変する必要があります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(require '[cljs.build.api :as b])

(b/watch (b/inputs "test" "src")
  {:main 'mytestingapp.core_tests
   :target :nodejs
   :output-to "out/mytestingapp.js"
   :output-dir "out"
   :verbose true})
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page99
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# This new script will compile and watch both directories "src" and "test", and it sets the new entry point to the mytestingapp.core_tests namespace.
この新しいスクリプトは、src ディレクトリと test ディレクトリの両方をコンパイルして監視し、新しいエントリーポイントを 名前空間 mytestingapp.core_tests に設定します。

#@# Next, put some test code in the core_tests.cljs file:
次に、core_tests.cljs にテスト・コードを追加します。

//emlist{
(ns mytestingapp.core-tests
  (:require [cljs.test :as t]))

(enable-console-print!)

(t/deftest my-first-test
  (t/is (= 1 2)))

(set! *main-cli-fn* #(t/run-tests))
//}

#@# The relevant part of that code snippet is:
このコードスニペットに関連する部分は以下の通りです。

//emlist{
(t/deftest my-first-test
  (t/is (= 1 2)))
//}

#@# The deftest macro is a basic primitive for defining our tests. It takes a name as its first parameter, followed by one or multiple assertions using the is macro. In this example, we try to assert that (= 1 2) is true.
deftestマクロは、テストを定義するための基本的な要素です。最初のパラメータとして名前を取り、続いて is マクロを使用した 1 つ以上のアサーションを取ります。この例では、(=1 2) が真であることを assert しようとします。


#@# Let’s try to run this. First start the watch process:
これを実行してみましょう。まず、監視プロセスを開始します。

//cmd{
$ ./scripts/watch
Building ...
Copying jar:file:/home/niwi/.m2/repository/org/clojure/clojurescript/1.9.36/clojurescript-1.9.36.jar!/cljs/core.cljs to out/cljs/core.cljs
Reading analysis cache for jar:file:/home/niwi/.m2/repository/org/clojure/clojurescript/1.9.36/clojurescript-1.9.36.jar!/cljs/core.cljs
Compiling out/cljs/core.cljs
... done. Elapsed 3.862126827 seconds
Watching paths: /home/niwi/cljsbook/playground/mytestingapp/test, /home/niwi/cljsbook/playground/mytestingapp/src
//}

#@# When the compilation is finished, try to run the compiled file with nodejs:
コンパイルが完了したら、Node.js を使用して、コンパイルされたファイルの実行を試みます。

//cmd{
$ node out/mytestingapp.js
 Testing mytestingapp.core-tests
FAIL in (my-first-test) (cljs/test.js:374:14)
expected: (= 1 2)
  actual: (not (= 1 2))
Ran 1 tests containing 1 assertions.
1 failures, 0 errors.
//}


#@# Page99
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

#@# You can see that the expected assert failure is successfully printed in the console. To fix the test, just change the = with not= and run the file again:
予想される assert のエラーがコンソールに正常に出力されたことがわかります。テストを修正するには、not= を付けて = を変更して、ファイルを再度実行します。

//emlist{
$ node out/mytestingapp.js

Testing mytestingapp.core-tests

Ran 1 tests containing 1 assertions.
0 failures, 0 errors.
//}

#@# It is fine to test these kinds of assertions, but they are not very useful. Let’s go to test some application code. For this, we will use a function to check if a year is a leap year or not. Write the following content to the src/mytestingapp/core.clj file:
このようなアサーションのテストはあまり役に立ちません。アプリケーション・コードをテストしてみましょう。そのために、うるう年かどうかを調べる関数を使います。次の内容をsrc/mytestingapp/core.clj に書き込みます。

//emlist{
(defn leap?
  [year]
  (and (zero? (js-mod year 4))
       (pos? (js-mod year 100))
       (pos? (js-mod year 400))))
//}

#@# Next, write a new test case to check that our new leap? function works properly. Make the core_tests.cljs file look like:
次に、新しい leap? 関数が正しく動作するかを確認するために、新しいテスト・ケースを作成します。core_tests.cljs は次のようになります。

//emlist{
(ns mytestingapp.core-tests
  (:require [cljs.test :as t]
            [mytestingapp.core :as core]))

(enable-console-print!)

(t/deftest my-first-test
  (t/is (not= 1 2)))

(t/deftest my-second-test
  (t/is (core/leap? 1980))
  (t/is (not (core/leap? 1981))))

(set! *main-cli-fn* #(t/run-tests))
//}

#@# Run the compiled file again to see that there are now two tests running. The first test passes as before, and our two new leap year tests pass as well.
コンパイル済みのファイルを再度実行して、2 つのテストが実行されていることを確認します。最初のテストは以前と同じように合格し、2 つの新しいうるう年のテストも合格します。

#@# Page100
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

=== 非同期のテスト

#@# Async Testing

#@# One of the peculiarities of ClojureScript is that it runs in an asynchronous, single-threaded execution environment, which has its challenges.
ClojureScript の特徴の 1 つは、非同期のシングルスレッド実行環境で実行されることですが、これには課題があります。

#@# In an async execution environment, we should be able to test asynchronous functions. To this end, the ClojureScript testing library offers the async macro, allowing you to create tests that play well with asynchronous code.
非同期実行環境では、非同期関数をテストできるはずです。このため、ClojureScript のテストライブラリには非同期マクロが用意されており、非同期コードでうまく動作するテストを作成することができます。

#@# First, we need to write a function that works in an asynchronous way. For this purpose, we will create the async-leap? predicate that will do the same operation but asychronously return a result using a callback:
まず、非同期的に動作する関数を作成する必要があります。この目的のために、同じ操作を行うけども、コールバックを使用して非同期に結果を返す述部 async-leap? を作成します。

//emlist{
(defn async-leap?
  [year callback]
  (js/setImmediate
   (fn []
     (let [result (or (zero? (js-mod year 400))
                      (and (pos? (js-mod year 100))
                           (zero? (js-mod year 4))))]
       (callback result)))))
//}

#@# The JavaScript function setImmediate is used to emulate an asynchronous task, and the callback is executed with the result of that predicate.
JavaScript の関数 setImmediate は非同期タスクをエミュレートするために使用され、コールバックはその述部の結果によって実行されます。

#@# To test it, we should write a test case using the previously mentioned async macro:
これをテストするには、前述の async マクロを使用してテストケースを作成する必要があります。

//emlist{
(t/deftest my-async-test
  (t/async done
    (core/async-leap? 1980 (fn [result]
                             (t/is (true? result))
                             (done)))))
//}

#@# The done function exposed by the async macro should be called after the asynchronous operation is finished and all assertions have run.
非同期マクロによって公開される done 関数は、非同期操作が終了して、すべてのアサーションが実行された後に呼び出す必要があります。

#@# It is very important to execute the done function only once. Omitting it or executing it twice may cause strange behavior and should be avoided.
done 関数は一度だけ実行することが非常に重要です。省略、または 2 回実行すると、異常な動作が発生する可能性があるため、使用しないでください。

== CI との連携

#@# Integrating with CI

#@# Most continuous integration tools and services expect that test scripts you provide return a standard exit code. But the ClojureScript test framework cannot customize this exit code without some configuration, because JavaScript lacks a universal exit code API for ClojureScript to use.

大半の CI ツールおよびサービスは、提供するテストスクリプトが標準の終了コードを返すことを前提としています。しかし、JavaScript には ClojureScript が使用できる汎用の終了コード API がないため、ClojureScript のテストフレームワークでは、設定なしにこの終了コードをカスタマイズすることができません。

#@# Page101
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

#@# To fix this, the ClojureScript test framework provides an avenue for executing custom code after the tests are done. This is where you are expected to set the environment-specific exit code depending on the final test status: 0 for success, 1 for failure.
この問題を解決するため、ClojureScript のテストフレームワークは、テストが完了した後にカスタムコードを実行する手段を提供します。ここでは、最終テストのステータス (成功の場合は0、失敗の場合は1) に応じて、環境固有の終了コードを設定する必要があります。

#@# Insert this code at the end of core_tests.cljs:
このコードをcore_tests.cljs の最後に挿入します。

//emlist{
(defmethod t/report [::t/default :end-run-tests]
  [m]
  (if (t/successful? m)
    (set! (.-exitCode js/process) 0)
    (set! (.-exitCode js/process) 1)))
//}

#@# Now, you may check the exit code of the test script after running:
これで、テストスクリプトの終了コードを実行後に確認できます。

//cmd{
$ node out/mytestingapp.js
$ echo $?
//}

#@# This code snippet obviously assumes that you are running the tests using nodejs. If you are running your script in another execution environment, you should be aware of how you can set the exit code in that environment and modify the previous snippet accordingly.
このコード・スニペットは明らかに、Node.js を使用してテストを実行していることを前提としています。別の実行環境でスクリプトを実行している場合は、その環境で終了コードを設定する方法と、それに応じて前のスニペットを変更する方法を理解しておく必要があります。
