= はじめに

#@# Introduction

#@# //quote{Why are we doing this? Because Clojure _rocks_, and JavaScript _reaches_.- Rich Hickey//}

//quote{
なぜ ClojureScript をしてるかだって？Clojure が最高で JavaScript が流行ってるからさ。

- Rich Hickey
//}

@<embed>{|latex|\vspace{2\Cvs\}}

#@# ClojureScript is an implementation of the Clojure programming language that　targets JavaScript. Because of this, it can run in many different execution　environments including web browsers, Node.js, io.js and Nashorn.

ClojureScript は JavaScript をターゲットにした Clojure の実装です。JavaScript をターゲットにすることにより、ClojureScript で書かれたプログラムは、Webブラウザや Node.js、また io.js や Nashorn など、様々な実行環境で動作します。

#@# Unlike other languages that intend to _compile_ to JavaScript (like TypeScript,　FunScript, or CoffeeScript), ClojureScript is designed to use JavaScript like　bytecode.  It embraces functional programming and has very safe and consistent　defaults. Its semantics differ significantly from those of JavaScript.
他の JavaScript へコンパイルされる言語（TypeScript、FunScript、CoffeeScript 等）とは異なり、ClojureScript は JavaScript をバイトコードのように使います。ClojureScript は関数型プログラミングを受け入れており、デフォルトでとても安全で一貫性があります。そのセマンティクスは、JavaScript と全く異なります。

#@# Another big difference (and in our opinion an advantage) over other languages is　that Clojure is designed to be a guest. It is a language without its own virtual　machine that can be easily adapted to the nuances of its execution environment. This　has the benefit that Clojure (and hence ClojureScript) has access to all the　existing libraries written for the host language.
もう一つの大きな違いは(私たちの見解では長所ですが)、Clojure がゲスト言語になるように設計されていることです。Clojure はそれ自体に仮想マシンを持たない言語であり、実行環境の違いに応じて順応させることができます。これにより、Clojure（ClojureScript）はホスト言語で書かれた全ての既存ライブラリにアクセスすることができます。

#@# Before we jump in, let us summarize some of the core ideas that ClojureScript brings　to the table. Don't worry if you don't understand all of them right now, they'll　become clear throughout the book.
詳細に入る前に、ClojureScript が提供する素晴らしいアイデアのうち、中心的なアイデアをまとめます。全てを理解できなかったとしても大丈夫です。本書を読み進めるにつれて、各々の内容が明確になっていきます。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# - ClojureScript enforces the functional programming paradigm with its design decisions and idioms. Although being strongly opinionated about functional programming it's a pragmatic language rather than pursuing theoretical purity.
//noindent
- ClojureScript では、設計の決定と表現手法において、関数型プログラミングのパラダイムに従う必要があります。関数型プログラミングに固執しますが、理論的な純粋さを追求するような言語ではなく、実用的な言語です。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# - Encourages programming with immutable data, offering highly performant and state of the art immutable collection implementations.
//noindent
- ClojureScript では、不変なデータでプログラムを書くことが推奨されます。この不変なデータはパフォーマンスが高く、最新の手法で実装されています。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# - It makes a clear distinction of identity and its state, with explicit constructs for managing change as a series of immutable values over time.
//noindent
- ClojureScript では、identity(識別) と state(状態) を明確に区別します。変更を不変な値の一連の流れとして管理するために、明示的に状態を生成します。

//embed[latex]{
\enlargethispage{15mm}
//}

//embed[latex]{
\clearpage
//} 

#@# - It has type-based and value-based polymorphism, elegantly solving the expression problem.
//noindent
- ClojureScript には、値に基づくポリモーフィズムと、型に基づくポリモーフィズムがあり、Expression Problem を簡潔に解決することができます。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# - It is a Lisp dialect so programs are written in the programming language's own data structures, a property known as _homoiconicity_ that makes metaprogramming (programs that write programs) as simple as it can be.
//noindent
- ClojureScript は Lisp 方言なので、プログラムはプログラミング言語自体のデータ構造で書くことができます。同図像性として知られる性質により、メタプログラミング(つまりプログラムを書くプログラム)をできるだけ簡潔に行うことができます。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# These ideas together have a great influence in the way you design and implement　software, even if you are not using ClojureScript. Functional programming, decoupling　of data (which is immutable) from the operations to transform it, explicit idioms for　managing change over time and polymorphic constructs for programming to abstractions　greatly simplify the systems we write.
以上の概念を合わせることにより、たとえ ClojureScript を使わないとしても、ソフトウェアの実装方法は大きな影響を受けるでしょう。関数型プログラミング、(不変な)データと値の変更の分離、変更を管理する明示的なイディオム、抽象化へのプログラミングのためのポリモーフィズムの構成により、私たちが書くシステムをより簡潔にすることができます。

#@#?? polymorphic constructs

#@# [quote, Rich Hickey]We can make the same exact software we are making today with dramatically simplerstuff — dramatically simpler languages, tools, techniques, approaches.

@<embed>{|latex|\vspace{1\Cvs\}}

//quote{
シンプルなプログラミング言語、ツール、テクニック、アプローチを使うことで、日々開発しているソフトウェアと全く同じものを作ることができるのだ。

- Rich Hickey
//}

@<embed>{|latex|\vspace{1\Cvs\}}

#@# We hope you enjoy the book and ClojureScript brings the same joy and inspiration that　has brought to us.

私たちが ClojureScript から感じた楽しみや感銘をこの本から受けていただければ幸いです。
