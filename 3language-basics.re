= ClojureScript 入門 

#@# Page11

#@# Language (the basics)

#@# This chapter will be a little introduction to ClojureScript without assumptions about previous knowledge of the Clojure language, providing a quick tour over all the things you will need to know about ClojureScript and understand the rest of this book.
#@# You can run the code snippets in the online interactive repl: http://www.ClojureScript.io
本章では、ClojureScript の概要を見ていきます。Clojure の知識は想定していません。4 章以降を理解するために必要なことを簡潔に説明します。オンラインの REPL@<fn>{repl1} でコードの動作を確認することができます。

//footnote[repl1][https://www.ClojureScript.io]

==  Lisp 構文 の第一歩

#@# First steps with Lisp syntax

#@# Invented by John McCarthy in 1958, Lisp is one of the oldest programming languages that is rstill around. It has evolved into many derivatives called dialects, ClojureScript being one of them. It is a programming language written in its own data structures — originally lists enclosed in parentheses — but Clojure(Script) has evolved the Lisp syntax with more data structures, making it more pleasant to write and read.
Lisp は 1958 年に John McCarthy 氏 によって発明された最も古いプログラミング言語の 1 つです。現在も様々な分野で利用されており、Lisp 方言と呼ばれる多くの派生が進化してきました。CloureScript も Lisp 方言の 1 つです。元々 Lisp ではリストは丸括弧で囲われますが、Clojure(Script) では独自のデータ構造が実装されており、読み書きが容易です。

#@# A list with a function in the first position is used for calling a function in  ClojureScript . In the example below, we apply the addition function to three arguments. Note that unlike in other languages, + is not an operator but a function. Lisp has no operators; it only has functions.
ClojureScript では、リストの 1 番目に関数がある場合、関数を呼び出すために使われます。次の例では加算する関数を 3 つの引数に適用させています。他の言語とは異なり、@<code>{+} は演算子ではなく関数であることに注意してください。Lisp には演算子はなく、関数しかありません。

//emlist{
(+ 1 2 3)
;; => 6
//}

#@# In the example above, we're applying the addition function + to the arguments 1 , 2 and 3. ClojureScript allows many unusual characters like ? or - in symbol names, which makes it easier to read:
上の例では、加算するための関数である @<code>{+} を @<code>{1}, @<code>{2}, @<code>{3} に適用しています。ClojureScript では @<code>{?} や @<code>{-} のような文字をシンボルの名前として利用できます。このことにより、ClojureScript で書かれたプログラムは読みやすくなっています。

//emlist{
(zero? 0)
;; => true
//}

#@# To distinguish function calls from lists of data items, we can quote lists to keep them from being evaluated.  The quoted lists will be treated as data instead of as a function call:
関数の呼び出しとデータとを区別するために、リストの前にクオート @<code>{'} をつけます。クオートがつけられたリストは、関数の呼び出しではなく、データとして処理されます。


#@# Page12
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
'(+ 1 2 3)  ;; => (+ 1 2 3)
//}

#@# ClojureScript uses more than lists for its syntax. The full details will be covered later, but here is an example of the usage of a vector (enclosed in brackets) for defining local bindings:
ClojureScript ではリスト以外の構文も使います。詳細な説明は後の章で行いますが、ベクタの使い方を例として取り上げます。ベクタは角括弧 @<code>{[ ]} で囲み、ローカルな束縛を定義します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [x 1
      y 2 
      z 3]
  (+ x y z))
;; => 6
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# This is practically all the syntax we need to know for using not only  ClojureScript , but any  Lisp . Being written in its own data structures (often referred to as _homoiconicity_) is a great property since the syntax is uniform and simple; also, code generation via xref:macros-section[macros] is easier than in any other language, giving us plenty of power to extend the language to suit our needs.
以上が ClojureScript でプログラムを書く上で必要となる構文の全てです。他の Lisp 方言でプログラムを書く場合も同様です。独自のデータ構造でプログラミング言語が実装されていることは、構文に統一性があり simple なので、素晴らしい特徴だといえます。マクロでコードを生成することも他の言語と比べて簡単であり、私たちのニーズに合わせて言語を拡張するのに十分な力を与えてくれます。

== 基本的なデータ型

#@# The base data types

#@# The ClojureScript language has a rich set of data types like most programming languages. It provides scalar data types that will be very familiar to you, such as numbers, strings, and floats. Beyond these, it also provides a great number of others that might be less familiar, such as symbols, keywords, regexes (regular expressions), variables, atoms, and volatiles.
他のプログラミング言語と同様、ClojureScript には多くのデータ型が用意されています。ClojureScript には、数値、文字列、浮動小数などのスカラー型のデータ構造があります。これら以外にも、シンボル、キーワード、正規表現、変数、アトム、Volatile などのデータ構造もあります。

#@# ClojureScript embraces the host language, and where possible, uses the host's provided types. For example: numbers and strings are used as is and behave in the same way as in  JavaScript .
ClojureScript はホスト言語を採用し、可能な限りホスト言が提供する型を使用します。例えば、数値と文字列は、JavaScript の数値と文字列と同じように利用して、結果を得ることができます。

=== 数値

#@# Numbers

#@# In  ClojureScript , numbers include both integers and floating points. Keeping in mind that ClojureScript is a guest language that compiles to  JavaScript , integers are actually  JavaScript 's native floating points under the hood.
#@# As in any other language, numbers in ClojureScript are represented in the following ways:
ClojureScript において、数値は整数と浮動小数点数を含みます。ClojureScript は JavaScript にコンパイルされる言語だということを覚えておいてください。整数は JavaScript では浮動小数点数になります。他の言語と同じように、ClojureScript で数値は次のように表現されます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
23
+23
-100
1.7
-2 
33e8
12e-14
3.2e-4
//}


#@# Page13
#@# @<embed>{|latex|\vspace{-0.5\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== キーワード

#@# Keywords

#@# Keywords in ClojureScript are objects that always evaluate to themselves. They are usually used in <<maps-section,map data structures>> to efficiently represent the keys.
ClojureScript においてキーワードは、常にキーワードへ評価されるオブジェクトです。マップのデータ構造でキーを効率よく表すために使われます。

//emlist{
:foobar　　;; => :foobar
:2　　　　　;; => :2
:?　　　　　;; => :?
//}

#@# As you can see, the keywords are all prefixed with :, but this character is only part of the literal syntax and is not part of the name of the object.
#@# You can also create a keyword by calling the keyword function. Don't worry if you don't understand or are unclear about anything in the following example;<<function-section,functions>> are discussed in a later section.
キーワードはリテラルとしてコロン @<code>{:} を使います。コロンはオブジェクトの名前の一部ではありません。
@<code>{keyword} 関数によりキーワードを作成することも可能です。以下の例は、今は理解できなくても構いません。後に関数の章で取り上げます。

//emlist{
(keyword "foo")
;; => :foo
//}

#@# When prefixing keywords with a double colon ::, the keyword will be prepended by the name of the current namespace. Note that namespacing keywords affects equality comparisons.
キーワードをダブルコロン @<code>{::} で始めると、キーワードの前に現在の名前空間が付きます。名前空間付きのキーワードは、等価性の比較に影響を与えるので注意してください。

//emlist{
::foo
;; => :cljs.user/foo

(= ::foo :foo)
;; => false
//}

#@# Another alternative is to include the namespace in the keyword literal, this is useful when creating namespaced keywords for other namespaces:
別の方法としては、リテラルに名前空間を含める方法もあります。この方法は、他の名前空間に名前空間付きのキーワードを作成する際に役に立ちます。

//emlist{
:cljs.unraveled/foo
;; => :cljs.unraveled/foo
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# The keyword function has an arity- 2   var iant where we can specify the namespace as the first parameter:
@<code>{keyword} 関数は 2 つの引数をとることができます。第 1 引数で名前空間を指定します。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(keyword "cljs.unraveled" "foo")
;; => :cljs.unraveled/foo
//}


#@# Page14
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== シンボル

#@# Symbols

#@# Symbols in ClojureScript are very, very similar to *keywords* (which you now know about). But instead of evaluating to themselves, symbols are evaluated to something that they refer to, which can be functions,  var iables, etc.
ClojureScript のシンボルはキーワードとよく似ていますが、自分自身に対して評価されるのではなく、シンボルが参照している対象(関数や変数等)に対して評価されます。

#@# Symbols start with a non numeric character and can contain alphanumeric characters as well as *, +, !, -, _, ', and ? such as :
シンボルの名前はアルファベットで始まり、記号(@<code>{*}, @<code>{+}, @<code>{!}, @<code>{-}, @<code>{_}, @<code>{'}, @<code>{?}) を含むことができますが、数字で始めることはできません。

//emlist{
sample-symbol
othersymbol
f1 
my-special-swap!
//}

#@# Don't worry if you don't understand right away; symbols are used in almost all of our examples, which will give you the opportunity to learn more as we go on.
理解できなかったとしても気にしないでください。シンボルは様々な例で使われており、読み進むにつれて理解が深まります。

=== 文字列

#@# Strings

#@# There is almost nothing new we can explain about strings that you don't already know. In  ClojureScript , they work the same as in any other language. One point of interest, however, is that they are immutable.
#@# In this case they are the same as in  JavaScript :
文字列については特に説明することはありません。ClojureScript は、他の言語と同じように動作します。ただし、文字列が不変である点に注意してください。
この点は JavaScript と同じです。

//emlist{
"An example of a string"
//}

#@# One peculiar aspect of strings in ClojureScript is due to the language's Lisp syntax: single and multiline strings have the same syntax:
ClojureScript の文字列で特異な点は、Lisp の構文に由来するものです。文字列の構文は 1 行でも複数行でも同じです。

//emlist{
"This is a multiline
      string in ClojureScript."
//}

=== 文字

#@# Characters

#@# ClojureScript also  let s you write single characters using Clojure's character literal syntax.
リテラルで文字を書くこともできます。

//emlist{
\a        ; 小文字の文字
\newline  ; 改行文字
//}

#@# Since the host language doesn't contain character literals, ClojureScript characters are transformed behind the scenes into single character  JavaScript strings.
ホスト言語の JavaScript では文字のリテラルを使えないので、ClojureScript の文字は、JavaScript の文字列の 1 文字に変換されます。


#@# Page15
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//} 


=== コレクション

#@# Collections

#@# Another big step in explaining a language is to explain its collections and collection abstractions. ClojureScript is not an exception to this rule.
#@# ClojureScript comes with many types of collections. The main difference between ClojureScript collections and collections in other languages is that they are persistent and immutable.
#@# Before moving on to these (possibly) unknown concepts, we'll present a high-level overview of existing collection types in  ClojureScript .
あるプログラミング言語について詳しく知るためには、その言語でコレクションがどのように抽象化されているかを知る必要があり、ClojureScript にも当てはまります。
ClojureScript のコレクションには多くの型があります。ClojureScript が他の言語のコレクションと違うのは、コレクションが永続的で不変であることです。
詳しい説明に入る前に ClojureScript のコレクションに存在する型について概要を見ましょう。

===== リスト

#@# Lists

#@# This is a classic collection type in languages based on  Lisp . Lists are the simplest type of collection in  ClojureScript . Lists can contain items of any type, including other collections.
#@# Lists in ClojureScript are represented by items enclosed between parentheses:
リストは Lisp 系言語で典型的なコレクションの型です。リストは ClojureScript で最もシンプルなコレクションです。リストはどの型の要素を含むことができます。リストに他のコレクションを含むこともできます。リストは要素を丸括弧で囲みます。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
'(1 2 3 4 5)
'(:foo :bar 2)
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# As you can see, all list examples are prefixed with the ' char. This is because lists in  Lisp -like languages are often used to express things like function or macro calls. In that case, the first item should be a symbol that will evaluate to something callable, and the rest of the list elements will be function arguments. However, in the preceding examples, we don't want the first item as a symbol; we just want a list of items.
上記の例では、リストの先頭にクオート @<code>{'} がついています。これは Lisp 系言語では、リストが関数やマクロの呼び出しに使われるためです。リストを関数やマクロの呼び出しに使う場合、初めの要素は関数やマクロのシンボル、残りの要素はその引数である必要があります。上の例ではリストの先頭にクオートがついているため、要素のリストとして扱われます。

@<embed>{|latex|\vspace{-0.2\Cvs\}}
      
#@# The following example shows the difference between a list without and with the preceding single quote mark:
リスト先頭にクオート @<code>{'} があるかどうかで、動作がどう違うかを確認しましょう。

//emlist{
(inc 1)
;; => 2
      
'(inc 1)
;; => (inc 1)
//}

#@# As you can see, if you evaluate `(inc 1)` without prefixing it with ', it will resolve the inc symbol to the *inc* function and will execute it with 1 as the first argument, returning the value 2 .
先頭にクオートをつけずに @<code>{(inc 1)} として評価した場合、@<code>{inc} シンボルは inc 関数として評価されます。@<code>{1} が第 1 引数として評価されて、@<code>{2} という値が返ります。

#@# You can also explicitly create a list with the list function:
リストを作るために、明示的に @<code>{list} 関数を使うことも可能です。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(list 1 2 3 4 5)
;; => (1 2 3 4 5)

(list :foo :bar 2)
;; => (:foo :bar 2)
//}

#@# Lists have the peculiarity that they are very efficient if you access them sequentially or access their first elements, but a list is not a very good option if you need random (index) access to its elements.
リストは、前から順に要素にアクセスするには効率よく処理できますが、リスト内の要素に対してランダムに(もしくは特定のインデックスに)アクセスするのは不得意です。


#@# Page16
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


===== ベクタ

#@# Vectors

#@# Like lists, *vectors* store a series of values, but in this case, with very efficient index access to their elements, as opposed to lists, which are evaluated in order. Don't worry; in the following sections we'll go in depth with details, but at this moment, this simple explanation is more than enough.
#@# Vectors use square brackets for the literal syntax;  let 's see some examples:
リストのようにベクタも連続する値を保存しますが、インデックスを用いて効率よく要素にアクセスできます。要素が前から順に評価されるリストと対照的です。後に詳しく説明しますが、現状ではこの程度の理解で十分です。ベクタはリテラルのために角括弧を使います。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
[:foo :bar]
[3 4 5 nil]
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# Like lists, vectors can contain objects of any type, as you can observe in the preceding example.
#@# You can also explicitly create a vector with the vector function, but this is not commonly used in ClojureScript programs:
リストと同様に、ベクタはどの型のオブジェクトを含むことができます。@<code>{vector} 関数を用いてベクタを作成できますが、あまり一般的ではありません。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(vector 1 2 3)
;; => [1 2 3]

(vector "blah" 3.5 nil)
;; => ["blah" 3.5 nil]
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

===== マップ

#@# Maps

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# Maps are a collection abstraction that allow you to store key/value pairs. In other languages, this type of structure is commonly known as a hash-map or dict(dictionary). Map literals in ClojureScript are written with the pairs between curly braces.
マップはコレクションの 1 つであり、キーと値をセットで保存します。他の言語では、連想配列や辞書と言われるデータ構造です。

//emlist{
{:foo "bar", :baz 2 }
{:alphabet [:a :b :c]}
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# NOTE: Commas are frequently used to separate a key-value pair, but they are comp let ely optional. In ClojureScript syntax, commas are treated like spaces.
キーと値を分けるためにコンマを使うかどうかは任意です。ClojureScript の構文では、コンマはスペースとして処理されます。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# Like vectors, every item in a map literal is evaluated before the result is stored in a map, but the order of evaluation is not guaranteed.
ベクタのように、マップリテラルの各々の要素は、結果がマップに保存される前に毎回評価されます。ただし、要素が順に評価されるかは保証されません。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

===== セット

#@# Sets

#@# Sets store zero or more unique items of any type and are unordered. Like maps, they use curly braces for their literal syntax, with the difference being that they use a `#` as the leading character. You can also use the set function to convert a collection to a set:
最後はセットです。セットは任意の型のデータを 0 個以上保存することができますが、要素間に順序はありません。セットのためには、マップのように波括弧 @<code>${ }$ を使います。波括弧の前に シャープ @<code>{#} をつける点がマップとは異なります。@<code>{set}関数を用いてコレクションをセットに変換することもできます。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
#{1 2 3 :foo :bar}
;; => #{1 :bar 3 :foo 2}

(set [1 2 1 3 1 4 1 5])
;; => #{1 2 3 4 5}
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# In subsequent sections, we'll go in depth about sets and the other collection types you've seen in this section.
後のセクションでは、セットと他のコレクションの型について詳しくみます。


#@# Page17
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//} 


==  vars

#@#  vars

#@# ClojureScript is a mostly functional language that focuses on immutability. Because of that, it does not have the concept of  var iables as you know them in most other programming languages. The closest analogy to  variables are the  variables you define in algebra; when you say `x == 6` in mathematics, you are saying that you want the symbol x to stand for the number six.
ClojureScript は関数型プログラミング言語であり、不変性に重点をおいています。そのため、他の言語の変数にあたる概念がありません。代数学における変数にあたる概念はあります。つまり数学で@<code>{x = 6}というときには、@<code>{x}というシンボルが 6 を表すということを意味します。

#@# In  ClojureScript ,  var s are represented by symbols and store a single value together with metadata.
#@# You can define a var using the def special form:
ClojureScript において var はシンボルで表現されて、メタデータと一緒に 1 つの値を保存します。特殊形式の @<code>{def} を使うことで、var を 1 つ定義することができます。

//emlist{
(def x 2 2)
(def y [1 2 3])
//}

#@#  var s are always top level in the namespace (<<namespace-section,which we will explain later>>). If you use def in a function call, the var will be defined at the namespace level, but we do not recommend this - instead, you should use let to define  var iables within a function.
var は名前空間において常にトップレベルです。名前空間については後に詳しく説明します。もし@<code>{def}を関数の呼び出しの中で使う場合、var は名前空間のレベルに定義されますが、これは推奨されません。関数の中で変数を定義するには@<code>{let}を使うことが推奨されます。


== 関数

#@# Functions

=== はじめの一歩

#@# The first contact

#@# It's time to make things happen. ClojureScript has what are known as first class functions. They behave like any other type; you can pass them as parameters and you can return them as values, always respecting the lexical scope. ClojureScript also has some features of dynamic scoping, but this will be discussed in another section.
ClojureScript には第一級関数と呼ばれるものがあります。第一級の関数は、他の型と同じように使えます。例えば、常にレキシカルスコープを重視しながら、関数の引数として渡せたり、関数の返り値として利用することができます。ClojureScript におけるダイナミックスコープについては別のセクションで扱います。

#@# If you want to know more about scopes, this link:http://en.wikipedia.org/wiki/Scope_(computer_science)[Wikipedia article] is very extensive and explains different types of scoping.
スコープについて詳しく知るには、http://en.wikipedia.org/wiki/Scope を参考にしてください。様々なタイプのスコープについて詳細を説明しています。

#@# As ClojureScript is a Lisp dialect, it uses the prefix notation for calling a function:
ClojureScript は Lisp 方言の 1 つであり、関数の呼び出しは、次のように前置記法を用います。

//emlist{
(inc 1)
;; => 2 
//}

#@# In the example above, inc is a function and is part of the ClojureScript runtime, and 1 is the first argument for the inc function.
この例では@<code>{inc}は関数であり、実行時に利用できる関数の 1 つです。この場合、@<code>{1} は@<code>{inc}関数の第 1 引数です。

//emlist{
(+ 1 2 3)
;; => 6
//}

      
#@# Page18
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


#@# The + symbol represents an add function. It allows multiple parameters, whereas in ALGOL-type languages, + is an operator and only allows two parameters.
@<code>{+} シンボルは加算を意味します。複数の引数をとることができる点ができ、ALGOL型のプログラミング言語が 2 つの引数しかとることができないことと対照的です。

#@# The prefix notation has huge advantages, some of them not always obvious. ClojureScript does not make a distinction between a function and an operator; everything is a function. The immediate advantage is that the prefix notation allows an arbitrary number of arguments per "operator". It also comp let ely eliminates the problem of operator precedence.
前置記法には大きな利点があり、その中には必ずしも明白でないものもあります。ClojureScript では関数とオペレータを区別しません。全てが関数です。すぐに分かる長所としては、前置記法ではオペレータが任意の数の引数を許容することです。また、演算子の優先順位の問題も解消されます。

=== 自作関数の定義

#@# Defining your own functions

#@# You can define an unnamed (anonymous) function with the fn special form. This is one type of function definition; in the following example, the function takes two parameters and returns their average.
特殊形式の @<code>{fn} を用いると、無名関数を定義することができます。次の例では、関数は 2 つの引数を取り、それらの平均を返します。

//emlist{
(fn [param1 param2]
  (/ (+ param1 param2) 2.0))
//}

#@# You can define a function and call it at the same time (in a single expression):
関数を定義して即時に呼び出すことができます（1 つの式で）。

//emlist{
((fn [x] (* x x)) 5)
;; => 25
//}

#@#  let 's start creating named functions. But what does a _named function_ really mean? It is very simple; in  ClojureScript , functions are first-class and behave like any other value, so naming a function is done by simply binding the function to a symbol:
次に名前付き関数を作成しましょう。名前付き関数とはどういうことでしょうか。ClojureScript ではすごくシンプルです。関数は第一級クラスであり、他の全ての値と同じように振る舞います。関数をシンボルに束縛することにより、関数に名前をつけます。

//emlist{
(def square (fn [x] (* x x)))

(square 12)
;; => 144
//}

#@# ClojureScript also offers the defn macro as a little syntactic sugar for making function definition more idiomatic:
ClojureScript には、関数定義の糖衣構文として @<code>{defn} マクロがあります。

//emlist{
(defn square
  "特定の数字の2乗を返す"
  [x]
  (* x x))
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}      

#@# The string that comes between the function name and the parameter vector is called a _docstring_ (documentation string); programs that automatically create web documentation from your source files will use these docstrings.
関数名と引数の間にある文字列は docstring(documentation string) と呼ばれます。Webでドキュメントを自動生成するときに docstring が使われます。

#@#??docstring -> 孔雀本では「ドキュメント文字列」と訳されている。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# Page19
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== 多様な Arity をもつ関数

#@# Functions with multiple arities

#@# ClojureScript also comes with the ability to define functions with an arbitrary number of arguments. (The term _arity_ means the number of arguments that a function takes.) The syntax is almost the same as for defining an ordinary function, with the difference that it has more than one body.
#@# let 's see an example, which will explain it better:
ClojureScript は任意の数の引数をとる関数を定義することができます。( Arity という用語は関数が受ける引数の数を意味します) そのための構文は普通の関数定義と大体同じですが、複数の本体をもつ点が異なります。
例をみてみましょう。

@<embed>{|latex|\vspace{-0.4\Cvs\}}      

//emlist{
(defn myinc
  "自己定義版のパラメータ化された inc"
  ([x] (myinc x 1))
  ([x increment] (+ x increment)))
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}      

#@# This line: `([x] (myinc x 1))` says that if there is only one argument, call the function myinc  with that argument and the number 1 as the second argument. The other function body `([x increment] (+ x increment))` says that if there are two arguments, return the result of adding them.
#@# Here are some examples using the previously defined multi-arity function. Observe that if you call a function with the wrong number of arguments, the compiler will emit an error message.
@<code>{([x] (myinc x 1))} は、もし引数が 1 つの場合、@<code>{myinc} 関数に @<code>{x} を第 1 引数、@<code>{1} を第 2 引数として渡すということを表しています。@<code>{([x increment] (+ x increment))} は、もし引数が 2 つの場合、2 つの引数を足した結果を返します。
ここで定義した関数を使ってみましょう。引数の数を間違えて関数を呼び出すと、コンパイラからエラーメッセージが出力されることに注目してください。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(myinc 1)
;; => 2 

(myinc 1 3)
;; => 4

(myinc 1 3 3)
;; Compiler error
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# NOTE: Explaining the concept of "arity" is out of the scope of this book, however you can read about that in this link:http://en.wikipedia.org/wiki/Arity[Wikipedia article].
Arity の概念を本書で説明することは、本書の範囲を超えています。詳しくは Wikipedia の Arity のページを参照してください。

=== 可変長引数関数

#@#  variadic functions

#@# Another way to accept multiple parameters is defining  var iadic functions.  var iadic functions are functions that accept an arbitrary number of arguments:
可変長引数をとる関数を定義することでも、複数の引数を受け取る関数を定義することができます。可変長引数をとる関数は、任意の数の引数をとることができます。

#@# The way to denote a  var iadic function is using the & symbol prefix on its arguments vector.
可変長引数関数を定義するには @<code>{&} シンボルを引数のベクタにつけます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn my-variadic-set
  [& params]
  (set params))
      
(my-variadic-set 1 2 3 1)
;; => #{1 2 3}
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page20
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//} 


=== 無名関数の短い構文

#@# Short syntax for anonymous functions

#@# ClojureScript provides a shorter syntax for defining anonymous functions using the `#()` reader macro (usually leads to one-liners). Reader macros are "special" expressions that will be transformed to the appropriate language form at compile time; in this case, to some expression that uses the fn special form.
ClojureScript では、無名関数のためのシンプルな構文としてリーダマクロの @<code>{#()} を使うことができます(ワンライナーを書くためによく用いられます)。リーダーマクロとは、コンパイル時に適当なフォームに変換される「特別な」表現方法です。この場合、@<code>{fn}のフォームに変換されます。

//emlist{
(def average #(/ (+ %1 %2) 2))

(average 3 4)
;; => 3.5
//}

#@# The preceding definition is shorthand for:
この @<code>{average} の定義は、次のように展開されます。

//emlist{
(def average-longer (fn [a b] (/ (+ a b) 2)))

(average-longer 7 8)
;; => 7.5
//}

#@# The % 1 , % 2 ... %N are simple markers for parameter positions that are implicitly declared when the reader macro will be interpreted and converted to a fn expression.
@<code>{%1} や @<code>{%2} は引数の位置を示す印であり、リーダーマクロが@<code>{fn}の式に変換される時に暗黙的に宣言されます。

#@# If a function only accepts one argument, you can omit the number after the % symbol, e.g., a function that squares a number: `#(* % 1 % 1))` はcan be written `++#++(* % %))`.
もし関数が 1 つしか引数を取らない場合、@<code>{%} シンボルの後の数字は省略できます。例えば、@<code>{#(* %1 %1)} は @<code>{#(* % %)} と書くことができます。

#@# Additionally, this syntax also supports the  var iadic form with the %& symbol:
さらに @<code>{%&} シンボルにより可変長引数にすることができます。

//emlist{
(def my-variadic-set #(set %&))

(my-variadic-set 1 2 2)
;; => #{1 2}
//}


== フロー制御

#@# Flow control

#@# ClojureScript has a very different approach to flow control than languages like  JavaScript , C, etc.
ClojureScript のフロー制御は、JavaScript や C などとアプローチが異なります。

=== if による条件分岐

#@# Branching with if

#@#  let 's start with a basic one: if. In  ClojureScript , the if is an expression and not a statement, and it has three parameters: the first one is the condition expression, the second one is an expression that will be evaluated if the condition expression evaluates to logical true, and the third expression will be evaluated otherwise.
ClojureScript において @<code>{if} は文ではなく式であり、3 つの引数をとります。1 つ目は条件式、2 つ目は条件が真の場合に評価される式、3 つ目は条件が偽の場合に評価される式です。


#@# Page21
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(defn discount
  "1点以上の購入で5%割引されます"
  [quantity]
  (if (>= quantity 100)
    0.05
    0))

(discount 30)
;; => 0

(discount 130)
;; => 0.05
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}      
      
#@# The block expression do can be used to have multiple expressions in an if branch.  xref:block-section[do is explained in the next section].
@<code>{if} の条件分岐の中で複数の式を実行するには @<code>{do} を使います。@<code>{do}については次のセクションで説明します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

=== cond による条件分岐

@<embed>{|latex|\vspace{-0.4\Cvs\}}      

#@# Sometimes, the if expression can be slightly limiting because it does not have the "else if" part to add more than one condition. The cond macro comes to the rescue.
#@# With the cond expression, you can define multiple conditions:
@<code>{if}式には複数の条件を追加するための @<code>{else if} の部分がないため、多少制限されますが、@<code>{cond} マクロでこの点を解決できます。@<code>{cond}では複数の条件を定義できます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(defn mypos?
  [x]
  (cond
    (> x 0) "positive"
    (< x 0) "negative"
    :else "zero"))
      
(mypos? 0)　　;; => "zero"

(mypos? -2)　;; => "negative"
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}      
      
#@# Also, cond has another form, called condp, that works very similarly to the simple cond but looks cleaner when the condition (also called a predicate) is the same for all conditions:
また、@<code>{cond}には @<code>{condp} と呼ばれる別の形式もあり、単純な@<code>{cond}と非常によく似た働きをしますが、条件(述語とも呼ばれます)が全ての条件に対して同じ場合は、より綺麗に見えます。

#@# The line `condp = (keyword code)` means that, in each of the following lines, ClojureScript will apply the = function to the result of evaluating `(keyword code)`.
以下の各行では、ClojureScript は @<code>{(keyword code)} を評価した結果に @<code>{=} 関数を適用します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(defn translate-lang-code
  [code]
  (condp = (keyword code)
    :es "Spanish"
    :en "English"
    "Unknown"))
      
(translate-lang-code "en")
;; => "English"
      
(translate-lang-code "fr")
;; => "Unknown"
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page22   
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


=== case による条件分岐

#@# The case branching expression has a similar use as our previous example with condp. The main differences are that case always uses the == predicate/function and its branching values are evaluated at compile time. This results in a more performant form than cond or condp but has the disadvantage that the condition value must be static.
#@# Here is the previous example rewritten to use case:
@<code>{case} は、前例の@<code>{condp}と同様の使い方をします。主な違いは、常に @<code>{=} 述語(関数)が使用されて、その分岐値がコンパイル時に評価されることです。そのため @<code>{cond}や@<code>{condp}よりもパフォーマンスが良いですが、条件の値が静的でなければならないという欠点があります。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn translate-lang-code
  [code]
  (case code
    "es" "Spanish"
    "en" "English"
    "Unknown"))

(translate-lang-code "en")
;; => "English"

(translate-lang-code "fr")
;; => "Unknown"
//}


== 真偽判定

#@# Truthiness

#@# This is the aspect where each language has its own semantics (mostly wrongly). The majority of languages consider empty collections, the integer 0, and other things like this to be false.  In  ClojureScript , unlike in other languages, only two values are considered as false: nil and false. Everything else is treated as logical true.
真偽判定はプログラミング言語によりセマンティクスが異なります。多くの言語では、空のコレクションや整数の @<code>{0} 、またこれらと同様の値を偽と見なします。ClojureScript では @<code>{nil} と @<code>{false} だけが偽と見なされ、その他は論理的に真として扱われます。

#@# Jointly with the ability to implement the callable protocol (the IFn, explained more in detail later), data structures like sets can be used just as predicates, without need of additional wrapping them in a function:
callable プロトコル(@<code>{IFn} については後述)を実装する能力と一緒に、集合のようなデータ構造は、関数の中でそれらを追加的にラップする必要なしに、述語として使うことができます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def valid? #{1 2 3})

(filter valid? (range 110))
;; => (1 2 3)
//}

#@# This works because a set returns either the value itself for all contained elements or nil:
このように動作するのは、セットが全ての要素の値自体か @<code>{nil} を返すためです。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(valid? 1)
;; => 1 

(valid? 4)
;; => nil
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page23
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


== ローカル、ブロック、ループ

#@# Locals, Blocks, and Loops

=== ローカル

#@# Locals

#@# ClojureScript does not have the concept of  var iables as in ALGOL-like languages, but it does have locals. Locals, as per usual, are immutable, and if you try to mutate them, the compiler will throw an error.
ClojureScript には ALGOL のような変数の概念がありませんが、ローカル(local)はあります。ローカルは不変であり、変更しようとするとエラーが発生します。

#@# Locals are defined with the let expression. The expression starts with a vector as the first parameter followed by an arbitrary number of expressions. The first parameter (the vector) should contain an arbitrary number of pairs that give a _binding form_ (usually a symbol) followed by an expression whose value will be bound to this new local for the remainder of the let expression.
ローカルは @<code>{let} 式で定義されます。@<code>{let}式は、最初のパラメータとしてベクタで始まり、その後に任意の数の式が続きます。最初のパラメータのベクタには束縛フォームを与えて、その @<code>{let} 内のローカルで有効な名前と値のペアを宣言します。

//emlist{
(let [x (inc 1)
      y (+ x 1)]
  (println "Simple message from the body of a let")
  (* x y))
//}

#@# In the preceding example, the symbol x is bound to the value `(inc 1)`, which comes out to 2 , and the symbol y is bound to the sum of x and 1 , which comes out to 3. Given those bindings, the expressions `(println "Simple message from the body of a  let ")` and `(* x y)` are evaluated.
上の例では、シンボル @<code>{x} が @<code>{(inc 1)} の値に束縛されて、シンボル @<code>{y} が @<code>{x} と 1 の合計(つまり 3 )に束縛されます。これらの束縛を受けて、 @<code>{(println "Simple message from the body of a  let")} と @<code>{(* x y)} が評価されます。

=== ブロック

#@# In JavaScript , braces `{` and `}` delimit a block of code that “belongs together”. Blocks in ClojureScript are created using the do expression and are usually used for side effects, like printing something to the console or writing a log in a logger.
#@# A side effect is something that is not necessary for the return value.
JavaScript において波括弧 @<code>${ }$ は「共に属する」コードのブロックを決めます。ClojureScript では @<code>{do} を用いてブロックを作成します。@<code>{do}はコンソールの何かの結果を出力したり、ログを出力したりするような、副作用を伴う場合に使います。副作用とは、戻り値には不要なものをいいます。

#@# The do expression accepts as its parameter an arbitrary number of other expressions, but it returns the return value only from the last one:
@<code>{do}は任意の数の式を含むことができますが、最後に評価された式の値が返り値となります。

//emlist{
(do
  (println "hello world")
  (println "hola mundo")
  ;; この値は返却されずに捨てられます。
  (* 3 5)
  (+ 1 2))
;; hello world
;; hola mundo
;; => 3
//}

#@# The body of the let expression, explained in the previous section, is very similar to the do expression in that it allows multiple expressions. In fact, the let has an implicit do.
先ほど説明した@<code>{let}の本体は、複数の式をもつことができる点において @<code>{do} とよく似ています。実際、@<code>{let} は暗黙の @<code>{do} をもちます。


#@# Page24
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== ループ

#@# loops

#@# The functional approach of ClojureScript means that it does not have standard, well-known, statement-based loops such as for in  JavaScript . The loops in ClojureScript are handled using recursion.  Recursion sometimes requires additional thinking about how to model your problem in a slightly different way than imperative languages.
ClojureScript は関数型のアプローチを採用しているので、JavaScript での @<code>{for} のような一般的なループがありません。ClojureScript のループは再帰を使って処理されます。再帰を用いてプログラムを書くためには、命令型のプログラミング言語とは少し違った方法で問題をモデル化する必要があります。

#@#(参)モデル化: http://e-words.jp/w/%E3%83%A2%E3%83%87%E3%83%AA%E3%83%B3%E3%82%B0.html

#@# Many of the common patterns for which for is used in other languages are achieved through higher-order functions - functions that accept other functions as parameters.
他の言語で @<code>{for} が使われるパターンの多くは、高階関数を用いて置き換えることができます。高階関数とは関数を引数として受け取る関数です。

===== loop/recur によるループ

#@# Looping with loop/recur

#@# let 's take a look at how to express loops using recursion with the loop and recur forms.  loop defines a possibly empty list of bindings (notice the symmetry with  let ) and recur jumps execution back to the looping point with new values for those bindings.
#@#  let 's see an example:
では、@<code>{loop} と @<code>{recur} を用いた再帰でループを表現する方法を見てみましょう。
@<code>{loop} は、空の可能性のある束縛のリスト(@<code>{let}との対称性に注目してください)を定義して、繰り返し実行すると、それらの束縛の新しい値を使ってループの開始点に戻ります。例を見てみましょう。

//emlist{
(loop [x 0]
  (println "Looping with " x)
  (if (= x 2)
    (println "Done looping!")
    (recur (inc x))))
;; Looping with 0
;; Looping with 1 
;; Looping with 2 
;; Done looping!
;; => nil
//}

#@# In the above snippet, we bind the name x to the value 0 and execute the body. Since the condition is not met the first time, it's rerun with recur, incrementing the binding value with the inc function. We do this once more until the condition is met and, since there aren't any more recur calls, exit the loop.
上の例では、まず @<code>{x} を @<code>{0} に束縛して本体を実行します。@<code>{if} の条件が満たされていないので、@<code>{(recur (inc x))} で @<code>{inc} で @<code>{x} に @<code>{1} が加算されてループが再実行されます。条件が満たされると @<code>{recur} の呼び出しが止まり、ループが終了します。

#@# Note that loop isn't the only point we can recur to; using recur inside a function executes the body of the function recursively with the new bindings:
@<code>{recur} を使うことができるのは @<code>{loop} だけではありません。関数の中で @<code>{recur} を使用すると、新しい束縛値で本体が再帰的に実行されます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn recursive-function
  [x]
  (println "Looping with" x)
  (if (== x 2)
    (println "Done looping!")
    (recur (inc x))))

(recursive-function 0)
;; Looping with 0
;; Looping with 1 
;; Looping with 2 
;; Done looping!
;; => nil
//}


#@# Page25
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


===== 高階関数によるループの置き換え

#@# In imperative programming languages it is common to use for loops to iterate over data and transform it, usually with the intent being one of the following:
命令型プログラミング言語では、@<code>{for} で反復的にデータを変形しますが、次のような目的で利用することが多いです。
命令型プログラミング言語では、@<code>{for} ループを使用してデータを繰り返し変換することが一般的です。通常、次のいずれかを目的とします。

#@# - Transform every value in the iterable yielding another iterable
#@# - Filter the elements of the iterable by certain criteria
#@# - Convert the iterable to a value where each iteration depends on the result from the previous one
#@# - Run a computation for every value in the iterable

//embed[latex]{
\vspace{1em}
//}

- イテラブルの全ての値を、別のイテラブルに変形する

- ある条件でイテラブルの要素をフィルターする

- イテラブルの要素を順に処理をして 1 つの値に変換する

- イテラブルにある各々の値を計算する

//embed[latex]{
\vspace{1em}
//}

#@# The above actions are encoded in higher-order functions and syntactic constructs in  ClojureScript ;  let 's see an example of the first three.
上記の実行は、高階関数と ClojureScript の構文を用いて書くことができます。まず最初の3つの例を見ていきましょう。

#@# For transforming every value in an iterable data structure we use the map function, which takes a function and a sequence and applies the function to every element:
イテラブルにある全ての値を変換するには @<code>{map} 関数を使います。@<code>{map} 関数は、関数とシーケンスを引数にとり、関数をシーケンスの各要素に適用します。

//emlist{
(map inc [0 1 2])
;; => (1 2 3)
//}

#@# The first parameter for map can be _any_ function that takes one argument and returns a value.  For example, if you had a graphing application and you wanted to graph the equation `y&# 1 60;==&# 1 60;3x&# 1 60;+&# 1 60;5` for a set of _x_ values, you could get the _y_ values like this:
@<code>{map} 関数の初めのパラメータには、1 つの引数をとり 1 つの値を返す関数を指定します。例えば、グラフ作成のアプリケーションがあるとします。@<code>{y = 3x + 5} の式を @<code>{x} の値のセットに対して適用して @<code>{y} の値のセットをえるには、次のように書きます。

//emlist{
(defn y-value [x] (+ (* 3 x) 5))

(map y-value [1 2 3 4 5])
;; => (8 11 14 17 20)
//}

#@# If your function is short, you can use an anonymous function instead, either the normal or short syntax:
高階関数に渡す関数が短い場合は、無名関数を使うこともできます。無名関数には @<code>{fn} か @<code>{#} のどちらを用いても構いません。
 
//emlist{
(map (fn [x] (+ (* 3 x) 5)) [1 2 3 4 5])
;; => (8 1 1 1 4 1 7 2 0)

(map #(+ (* 3 %) 5) [1 2 3 4 5])
;; => (8 1 1 1 4 1 7 2 0)
//}

#@# For filtering the values of a data structure we use the filter function, which takes a predicate and a sequence and gives a new sequence with only the elements that returned true for the given predicate:
データ構造の値をフィルターするには、@<code>{filter} 関数を使います。@<code>{filter} 関数は、述語とシーケンスをとり、述語に対して真を返す要素だけを持つ新しいシーケンスを生成します。

#@# give の用法: 〈計算・分析などが〉〈…の〉結果を出す.2 times 5 gives 10. 5掛ける2は10

#@# Page26
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(filter odd? [1 2 3 4])
;; => (1 3)
//}

#@# Again, you can use any function that returns true or false as the first argument to filter.  Here is an example that keeps only words less than five characters long. (The count function returns the length of its argument.)
ここでも、@<code>{filter} 関数に適用する最初の引数として @<code>{true} または @<code>{false} を返す任意の関数を使用できます。5 文字未満の単語のみを保つ例をみてみましょう(@<code>{count} 関数は引数の長さを返します)。

//emlist{
(filter (fn [word] (< (count word) 5)) ["ant" "baboon" "crab" "duck" "echidna" "fox"])
;; => ("ant" "crab" "duck" "fox")
//}

#@# Converting an iterable to a single value, accumulating the intermediate result at every step of the iteration can be achieved with reduce, which takes a function for accumulating values, an optional initial value and a collection:
イテラブルの要素を 1 つずつ処理をして蓄積しながら 1 の値を返すには @<code>{reduce} を用います。@<code>{reduce} は値を蓄積するための関数、初期値、コレクションを取りますが、初期値については任意です。

//emlist{
(reduce + 0 [1 2 3 4])
;; => 10
//}

#@# Yet again, you can provide your own function as the first argument to reduce , but your function must have _two_ parameters. The first one is the "accumulated value" and the second parameter is the collection item being processed. The function returns a value that becomes the accumulator for the next item in the list.  For example, here is how you would find the sum of squares of a set of numbers (this is an important calculation in statistics). Using a separate function:
また、@<code>{reduce} の最初の引数として自作の関数を指定することもできますが、その関数には 2 つの引数が必要です。最初の引数は「蓄積値」で、 2番目の引数は処理されるコレクションのアイテムです。この関数は、リスト内の次のアイテムのアクミュレータになる値を返します。例えば、数値の集合の平方和を求めましょう（統計では重要な計算です）。関数を分ける場合は次のようにします。

//emlist{
(defn sum-squares
  [accumulator item]
  (+ accumulator (* item item)))

(reduce sum-squares 0 [3 4 5])
;; => 50
//}

#@# ...and with an anonymous function:
無名関数を使う場合は次のようにします。

//emlist{
(reduce (fn [acc item] (+ acc (* item item))) 0 [3 4 5])
;; => 50
//}

#@# Here is a reduce that finds the total number of characters in a set of words:
単語セット内の総文字数を検出する @<code>{reduce} 関数を次に示します。

#@# Page27
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(reduce (fn [acc word] (+ acc (count word)))
        0 ["ant" "bee" "crab" "duck"])
;; => 14
//}

#@# We have not used the short syntax here because, although it requires less typing, it can be less readable, and when you are starting with a new language, it's important to be able to read what you wrote! If you are comfortable with the short syntax, feel free to use it.
上の例では、短い構文を使っていません。なぜなら、短い構文を使うとタイピング数を減らせますが、コードの可読性が低下する可能性があるからです。新しい言語を始めるときは、自分が書いたものを後で読めることは重要です。もし短い構文に慣れている場合は、この構文を使用してください。

#@# Remember to choose your starting value for the accumulator carefully. If you wanted to use reduce to find the product of a series of numbers, you would have to start with one rather than zero, otherwise all the numbers would be multiplied by zero!
アキュムレータに渡す開始値は慎重に選ぶ必要があることを覚えておいてください。一連の数の積を求めるために @<code>{reduce} を使うときは、0 ではなく 1 を開始値として使う必要があります。0 を用いて掛け算をすることができないためです。

//emlist{
;; wrong starting value
(reduce * 0 [3 4 5])　　;; => 0
      
;; correct starting accumulator
(reduce * 1 [3 4 5])　　;; => 60
//}

===== @<code>{for} によるシーケンス内包表記

#@# In  ClojureScript , the for construct isn't used for iteration but for generating sequences, an operation also known as "sequence comprehension". In this section we'll learn how it works and use it to declaratively build sequences.
ClojureScript の @<code>{for} は、反復のためではなくシーケンスを生成するために用いられます。これはシーケンス内包表記として知られています。このセクションでは、シーケンス内包表記がどのように動作するかを学び、シーケンスを宣言的に構築するためにシーケンス内包表記を使います。

#@# for takes a vector of bindings and an expression and generates a sequence of the result of evaluating the expression. let 's take a look at an example:
@<code>{for} は束縛のためのベクタと式をとり、式が評価された結果をシーケンスとして生成します。

//emlist{
(for [x [1 2 3]]
  [x (* x x)])
;; => ([1 1] [2 4] [3 9])
//}

#@# In this example, x is bound to each of the items in the vector `[1 2 3]` in turn, and returns a new sequence of two-item vectors with the original item squared.
この例では、@<code>{x} は @<code>{[1 2 3]} の各要素に順に束縛され、元の要素と それを 2 乗した値をもつ 2 要素ベクタの新しいシーケンスを返します。

#@# for supports multiple bindings, which will cause the collections to be iterated in a nested fashion, much like nesting for loops in imperative languages. The innermost binding iterates “fastest.”
@<code>{for}は複数の束縛をサポートしているため、命令型言語での @<code>{for} ループのネストと同様に、コレクションがネストされて反復されます。最も内側の束縛が「最も速く」繰り返されます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(for [x [1 2 3]
      y [4 5]]
  [x y])
;; => ([1 4] [1 5] [2 4] [2 5] [3 4] [3 5])
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page28
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# We can also follow the bindings with three modifiers: : let for creating local bindings, :while for breaking out of the sequence generation, and :when for filtering out values.
さらに 3 つの修飾子があります。@<code>{:let} はローカルな束縛の作成のために、@<code>{:while} はシーケンス生成からの脱出のために、@<code>{:when} は値のフィルタリングのために用います。

#@# Here's an example of local bindings using the :let modifier; note that the bindings defined with it will be available in the expression:
次は @<code>{:let} を用いたローカル束縛の例です。@<code>{:let} で定義された束縛はその式でのみ利用可能です。

//emlist{
(for [x [1 2 3]
      y [4 5]
      :let [z (+ x y)]]
  z)      
;; => (5 6 6 7 7 8)
//}

#@# We can use the :while modifier for expressing a condition that, when it is no longer met, will stop the sequence generation. Here's an example:
@<code>{:while} 修飾子は、条件が合わなくなったときにシーケンスの生成を止める条件を表すために使います。次の例を見てください。

//emlist{
(for [x [1 2 3]
      y [4 5]
      :while (= y 4)]
  [x y])
;; => ([1 4] [2 4] [3 4])
//}

#@# For filtering out generated values, use the :when modifier as in the following example:
生成された値をフィルターするには、次のように @<code>{:when} 修飾子を使います。

//emlist{
(for [x [1 2 3]
      y [4 5]
      :when (= (+ x y) 6)]
  [x y])
;; => ([1 5] [ 2  4])
//}

#@# We can combine the modifiers shown above for expressing complex sequence generations or more clearly expressing the intent of our comprehension:
上記のような修飾語を組み合わせることで、複雑な配列の生成を表現したり、理解の意図をより明確に表現することができます。

//emlist{
(for [x [1 2 3]
      y [4 5]
      :let [z (+ x y)]
      :when (= z 6)]
  [x y])
;; => ([1 5] [2 4])
//}

#@# When we outlined the most common usages of the for construct in imperative programming languages, we mentioned that sometimes we want to run a computation for every value in a sequence, not caring about the result. Presumably we do this for achieving some sort of side-effect with the values of the sequence.
命令型プログラミング言語における @<code>{for} 構文の最も一般的な使用方法を説明した際に、結果を気にせずにシーケンスのすべての値に対して計算を実行したい場合があると述べました。おそらく、シーケンスの値を用いて何らかの副次的効果を得るために、このようなことをするのでしょう。

#@# Page29
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

#@# ClojureScript provides the doseq construct, which is analogous to for but executes the expression, discards the resulting values, and returns nil.
同様の目的を達成するために、 ClojureScript には @<code>{doseq} 構文があります。@<code>{doseq} は @<code>{for} と似ていますが、式を実行した後、結果の値を捨てて @<code>{nil} を返します。

//emlist{
(doseq [x [1 2 3]
        y [4 5]
       :let [z (+ x y)]]
;; 1 + 4 = 5
;; 1 + 5 = 6
;; 2 + 4 = 6
;; 2 + 5 = 7
;; 3 + 4 = 7
;; 3 + 5 = 8
;; => nil
//}

#@# If you want just iterate and apply some side effectfull operation (like println) over each item in the collection, you can just use the specialized function run! that internally uses fast reduction:
コレクション内の各アイテムに対して副作用のある操作 (@<code>{println}等) を効率よく繰り返し適用したい場合は、それに特化した関数 @<code>{run!} を使います。内部的には高速に @<code>{reduce} を使用します。

//emlist{
(run! println [1 2 3])
;; 1 
;; 2 
;; 3
;; => nil
//}

#@# This function explicitly returns nil.
この関数は明示的に @<code>{nil} を返します。


== コレクションの型

#@# Collection types

=== 不変性と永続性

#@# Immutable and persistent

#@# We mentioned before that ClojureScript collections are persistent and immutable, but we didn't explain what that meant.
#@# An immutable data structure, as its name suggests, is a data structure that cannot be changed. In-place updates are not allowed in immutable data structures.
#@#  let 's illustrate that with an example: appending values to a vector using the conj (conjoin) operation.
ClojureScript のコレクションは永続的で不変だと言及しましたが、詳しく説明していませんでした。不変のデータ構造とは、変更ができないデータ構造です。不変なデータ構造においては、部分的な更新は許されません。@<code>{conj} を用いてベクタに値を付け加える例を見てみましょう。

//emlist{
(let [xs [1 2 3]
      ys (conj xs 4)]
  (println "xs:" xs)
  (println "ys:" ys))
;; xs: [1 2 3]
;; ys: [1 2 3 4]
;; => nil
//}


#@# Page30
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


#@# As you can see, we derived a new version of the xs vector appending an element to it and got a new vector ys with the element added. However, the xs vector remained unchanged because it is immutable.
上の例では @<code>{xs} に要素を 1 つ追加して新たなベクタ @<code>{ys} を作成しています。ベクタ @<code>{xs} は不変なので変更されません。

#@# A persistent data structure is a data structure that returns a new version of itself when transforming it, leaving the original unmodified. ClojureScript makes this memory and time efficient using an implementation technique called _structural sharing_, where most of the data shared between two versions of a value is not duplicated and transformations of a value are implemented by copying the minimal amount of data required.
永続的なデータ構造とは、変換時に元のバージョンを残したまま新しいバージョンを返すデータ構造のことです。ClojureScript では、構造共有 (structural sharing) と呼ばれる実装技術を用いて、メモリと時間の効率化を図っており、2 つのバージョンの値の間で共有されるデータのほとんどは重複しません。

#@# If you want to see an example of how structural sharing works, read on. If you're not interested in more details you can skip over to the xref:the-sequence-abstraction[next section].
構造共有がどのように動作しているかを知りたい場合は読み進めてください。もし興味がない場合は、読み飛ばして次のセクション(シーケンスの抽象化)に進んでください。

#@# For illustrating the structural sharing of ClojureScript data structures,  let 's compare whether some parts of the old and new versions of a data structure are actually the same object with the identical? predicate. We'll use the list data type for this purpose:
ClojureScript におけるデータ構造の構造共有を説明するために、古いデータ構造と新しいデータ構造の一部が同じオブジェクトかどうかを @<code>{identical?} を用いて比較してみます。ここではリストを例に説明します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [xs (list 1 2 3)
      ys (cons 0 xs)]
  (println "xs:" xs)
  (println "ys:" ys)
  (println "(rest ys):" (rest ys))
  (identical? xs (rest ys)))
;; xs: (1 2 3)
;; ys: (0 1 2 3)
;; (rest ys): (1 2 3)
;; => true
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# As you can see in the example, we used cons (construct) to prepend a value to the xs list and we got a new list ys with the element added. The rest of the ys list (all the values but the first) are the same object in memory as the xs list, thus xs and ys share structure.
リストの @<code>{xs} に @<code>{cons} 関数を使って値を追加して新たなリスト @<code>{ys} を作成しています。リスト @<code>{ys} の @<code>{rest} はリスト @<code>{xs} とメモリでは等しくなります。この挙動を見ると、リスト @<code>{xs} と リスト @<code>{ys} がデータ構造を共有していることがわかります。


=== シーケンスの抽象化

#@# The sequence abstraction

#@# One of the central ClojureScript abstractions is the _sequence_ which can be thought of as a list and can be derived from any of the collection types. It is persistent and immutable like all collection types, and many of the core ClojureScript functions return sequences.
ClojureScript による抽象化で中心的な概念の 1 つに シーケンス(sequence) があります。シーケンスはリストとして見なされ、どのコレクションの型もシーケンスとして見なすことができます。シーケンスは全てのコレクションの型のように永続的で変更不可能です。大半の ClojureScript の関数はシーケンスを返します。

#@# The types that can be used to generate a sequence are called "seqables"; we can call seq on them and get a sequence back. Sequences support two basic operations: first and rest. They both call seq on the argument we provide them:
シーケンスを生成するために使うことができる型はシーカブル(seqables)と呼ばれます。@<code>{seq} を シーカブルに対して呼び出して、シーケンスをえることができます。シーケンスは 基本的な関数 @<code>{first} と @<code>{rest} をサポートします。どちらも @<code>{first} と @<code>{rest} に与える引数に対して @<code>{seq} を呼び出します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(first [1 2 3])
;; => 1 
      
(rest [1 2 3])
;; => (2 3)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}      

#@# Page32
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

#@# Calling seq on a seqable can yield different results if the seqable is empty or not. It will return nil when empty and a sequence otherwise:
@<code>{seq} 関数をシーカブルに対して呼ぶとき、シーカブルが空かどうかで結果が異なります。空の場合には @<code>{nil} を、そうでなければシーケンスを返します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(seq [])
;; => nil

(seq [1 2 3])
;; => (1 2 3)
//}
      
@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# next is a similar sequence operation to rest, but it differs from the latter in that it yields a nil value when called with a sequence with one or zero elements. Note that, when given one of the aforementioned sequences, the empty sequence returned by rest will evaluate as a boolean true whereas the nil value returned by next will evaluate as false (xref:truthiness-section[see the section on _truthiness_ later in this chapter]).
@<code>{next} は @<code>{rest} と似たシーケンス操作関数ですが、シーケンスの要素が空か 1 つの場合に @<code>{nil} を返す点が @<code>{rest} とは異なります。前述のシーケンスの 1 つが与えられたとき、@<code>{rest} により返される空のシーケンスは論理値で真を返します。一方、@<code>{next} により返される @<code>{nil} の値は偽と評価されます。真偽判定の章で詳しく説明します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(rest [])
;; => ()
      
(next [])
;; => nil

(rest [1 2 3])
;; => (2 3)
      
(next [1 2 3])
;; => (2 3)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

===== nil パンニング

#@# nil-punning

#@# Since seq returns nil when the collection is empty, and nil evaluates to false in boolean context, you can check to see if a collection is empty by using the seq function. The technical term for this is nil-punning.
@<code>{seq} はコレクションが空のときに @<code>{nil} を返し、@<code>{nil} は論理値として偽と評価されるので、コレクションが空かどうかを @<code>{seq} 関数を用いて確認することができます。このようなテクニックは @<code>{nil} パンニングと呼ばれます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(defn print-coll
  [coll]
  (when (seq coll)
    (println "Saw " (first coll))
    (recur (rest coll))))

(print-coll [1 2 3])
;; Saw 1 
;; Saw 2 
;; Saw 3
;; => nil

(print-coll #{1 2 3})
;; Saw 1 
;; Saw 3
;; Saw 2 
;; => nil
//}


#@# Page32
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# Though nil is neither a seqable nor a sequence, it is supported by all the functions we saw so far:
@<code>{nil} は シーカブル でもシーケンスでもありませんが、これまで見てきた全ての関数でサポートされています。

//emlist{
(seq nil)
;; => nil

(first nil)
;; => nil

(rest nil)
;; => ()
//}


===== シーケンス操作関数

#@# Functions that work on sequences

#@# The ClojureScript core functions for transforming collections make sequences out of their arguments and are implemented in terms of the generic sequence operations we learned about in the preceding section. This makes them highly generic because we can use them on any data type that is seqable.  let 's see how we can use map with a  var iety of seqables:
ClojureScript コレクションを変換するための中心的な関数は、引数からシーケンスの生成を行い、前章で学んだ一般的なシーケンス操作関数において使えるように実装されています。このことにより、それらの関数は、全ての seqable なデータ型に対して使うことができます。次の例で @<code>{map} 関数が様々な seqables に対して使えることを見てみましょう。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(map inc [1 2 3])
;; => (2 3 4)

(map inc #{1 2 3})
;; => (2 4 3)

(map count {:a 41 :b 40})

(map inc '(1 2 3))
;; => (2 3 4)
//}

//embed[latex]{
\vspace{-0.2\Cvs}
//} 
 
#@# NOTE: When you use the map function on a map collection, your higher-order function will receive a two-item vector containing a key and value from the map. The following example uses xref:destructuring-section[destructuring] to access the key and value.
@<code>{map} 関数をマップのコレクションに対して使うとき、高階関数はキーと値を含む 2 つのアイテムからなるベクタを受け取ります。次の例では、キーと値にアクセスするために destructuring を使います。

//embed[latex]{
\vspace{-0.2\Cvs}
//}

//emlist{
(map (fn [[key value]] (* value value))
     {:ten 10 :seven 7 :four 4})
;; => (100 49 16)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Obviously the same operation can be done in more idiomatic way only obtaining a seq of values:
値のシーケンスを取得するために限れば、同じ操作をより慣用的な方法で行うことも可能です。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(map (fn [value] (* value value))
     (vals {:ten 10 :seven 7 :four 4}))
;; => (100 49 16)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page32
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# As you may have noticed, functions that operate on sequences are safe to use with empty collections or even nil values since they don't need to do anything but return an empty sequence when encountering such values.
気がついたかもしれませんが、シーケンスを処理する関数は、空のシーケンスを返すだけでいいので、空のコレクションや @<code>{nil} 値であっても安全に使用することができます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}
 
//emlist{
(map inc [])　 ; ==> ()

(map inc #{})　; ==> ()

(map inc nil)　; ==> ()
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# We already saw examples with the usual suspects like map, filter, and reduce, but ClojureScript offers a p let hora of generic sequence operations in its core namespace. Note that many of the operations we'll learn about either work with seqables or are extensible to user-defined types.
これまで @<code>{map}, @<code>{filter}, @<code>{reduce} などの一般的な使用例はすでに紹介しましたが、 ClojureScript の core 名前空間には、さらに多くの汎用のシーケンス操作関数が用意されています。ここで説明する操作の多くは、シーカブルで動作するか、もしくはユーザ定義型に拡張可能です。

#@# We can query a value to know whether it's a collection type with the coll? predicate:
@<code>{coll?} 述語を用いて、データがコレクション型かどうかを判別できます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(coll? nil)　
;; => false

(coll? [1 2 3])
;; => true

(coll? {:language "ClojureScript" :file-extension "cljs"})
;; => true

(coll? "ClojureScript")
;; => false
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Similar predicates exist for checking if a value is a sequence (with seq?) or a seqable (with seqable?):
同様に、データ構造がシーケンスかどうかを判別する @<code>{seq?} 述語、シーカブルかどうかを判別したりする@<code>{seqable?} 述語があります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(seq? nil)
;; => false
(seqable? nil)
;; => false

(seq? [])
;; => false
(seqable? [])
;; => true

(seq? #{1 2 3})
;; => false
(seqable? #{1 2 3})
;; => true

(seq? "ClojureScript")
;; => false
(seqable? "ClojureScript")
;; => false
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# For collections that can be counted in constant time, we can use the count operation. This operation also works on strings, even though, as you have seen, they are not collections, sequences, or seqable.
一定時間内にカウントできるコレクションの場合は、@<code>{count} 操作を使用できます。この操作は文字列に対しても機能しますが、これまで見てきたように、文字列はコレクション、シーケンス、またはシーカブルではありません。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{ 
(count nil)
;; => 0

(count [1 2 3])
;; => 3

(count {:language "ClojureScript" :file-extension "cljs"})
;; => 2 

(count "ClojureScript")
;; => 13
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# We can also get an empty  variant of a given collection with the empty function:
また @<code>{empty} 関数を使うことで、与えられたコレクションの空の variant を得ることもできます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(empty nil)
;; => nil

(empty [1 2 3])
;; => []

(empty #{1 2 3})
;; => #{}
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# The empty? predicate returns true if the given collection is empty:
@<code>{empty?} 述語は、コレクションが空の場合に @<code>{true} を返します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(empty? nil)
;; => true

(empty? [])
;; => true

(empty? #{1 2 3})
;; => false
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# The conj operation adds elements to collections and may add them in different "places" depending on the type of collection. It adds them where it is most performant for the collection type, but note that not every collection has a defined order.
@<code>{conj} 操作は要素をコレクションに追加しますが、コレクションの型に応じて異なる「場所」に追加することができます。コレクションの型で最もパフォーマンスが高い場所に要素を追加します。ただし、全てのコレクションに順序が定義されているわけではありません。

#@# We can pass as many elements as we want to add to conj;  let 's see it in action:
@<code>{conj} には追加する要素をいくつでも渡すことができます。実際の動作を見てみましょう。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(conj nil 4 2)
;; => (4 2)
//}

#@# Page35
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

//emlist{
(conj [1 2] 3)
;; => [1 2 3]

(conj [1 2] 3 4 5)
;; => [1 2 3 4 5]

(conj '(1 2) 0)
;; => (0 1 2)

(conj #{1 2 3} 4)
;; => #{1 3 2 4}

(conj {:language "ClojureScript"} [:file-extension "cljs"])
;; => {:language "ClojureScript", :file-extension "cljs"}
//}

===== 遅延

#@# Laziness

#@# Most of  ClojureScript 's sequence-returning functions generate lazy sequences instead of eagerly creating a whole new sequence. Lazy sequences generate their contents as they are requested, usually when iterating over them. Laziness ensures that we don't do more work than we need to and gives us the possibility of treating potentially infinite sequences as regular ones.
ClojureScript のシーケンスを返す大半の関数は、全く新たなシーケンスを生成するのではなく、遅延シーケンスを生成します。遅延シーケンスは、要求がされたときに内容を生成します。通常は、それらに反復作業を行うときに生成されます。遅延により、必要以上のことをしないことが保証して、潜在的な無限シーケンスを正規のシーケンスとして扱うことができます。

#@# Consider the range function, which generates a range of integers:
整数の範囲を生成する range 関数を見てみましょう。

//emlist{
(range 5)
;; => (0 1 2 3 4)

(range 1 10)
;; => (1 2 3 4 5 6 7 8 9)

(range 10 100 15)
;; (10 25 40 55 70 85)
//}

#@# If you just say (range), you will get an infinite sequence of all the integers. Do *not* try this in the REPL, unless you are prepared to wait for a very, very long time, because the REPL wants to fully evaluate the expression.
@<code>{(range)} だけで呼び出した場合、整数の無限シーケンスを生成します。REPLで @<code>{(range)} を試さないでください。REPLはその式を完全に評価しようとするので、とても長い間待たされます。

#@# Here is a contrived example.  let 's say you are writing a graphing program and you are graphing the equation _y_== 2 _x_ ^ 2 ^ + 5, and you want only those values of _x_ for which the _y_ value is less than 1 00. You can generate all the numbers 0 through 1 00, which will certainly be enough, and then take-while the condition holds:
少し例を見てみましょう。グラフ作成のプログラムを書いていて、@<code>{y = 2 x ^ 2 + 5} のグラフを書くとします。@<code>{y} の値が @<code>{100}未満の場合の @<code>{x} の値を求めます。@<code>{0} から @<code>{100} までの全ての数を生成して、@<code>{take-while} を用いて条件に合うものを保ちます。
ここに例を見てみましょう。グラフ作成プログラムを作成していて、方程式 @<code>{y = 2 x ^ 2 + 5} をグラフ化していて、@<code>{x} の値のうち、 @<code>{y} の値が @<code>{100} 未満の値だけが必要だとします。@<code>{0} から @<code>{100} までの全ての数を生成して、条件が満たされている間だけ値をとります。

//emlist{
(take-while (fn [x] (< (+ (* 2 x x) 5) 100))
            (range 0 100))
;; => (0 1 2 3 4 5 6)
//}

#@# Page37
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}
 
 
=== コレクション詳細

#@# Collections in depth

#@# Now that we're acquainted with  ClojureScript 's sequence abstraction and some of the generic sequence manipulating functions, it's time to dive into the concrete collection types and the operations they support.
ここまで、ClojureScript におけるシーケンスの抽象化とシーケンス操作関数について理解を深めました。次は、コレクション型の詳細と、それらがサポートする関数について見ていきます。

===== リスト

#@# Lists

#@# In  ClojureScript , lists are mostly used as a data structure for grouping symbols together into programs. Unlike in other  Lisp s, many of the syntactic constructs of ClojureScript use data structures different from the list (vectors and maps).  This makes code less uniform, but the gains in readability are well worth the price.
ClojureScriptでは、シンボルをグループ化してプログラムにするためのデータ構造として、主にリストが使用されます。他の Lisp 方言とは異なり、ClojureScript ではリストとは異なるデータ構造体を使用します(ベクタとマップ)。このことによりコードの統一感が低下しますが、コードの読みやすさは向上します。

#@# You can think of ClojureScript lists as singly linked lists, where each node contains a value and a pointer to the rest of the list. This makes it natural (and fast!) to add items to the front of the list, since adding to the end would require traversal of the entire list. The prepend operation is performed using the cons function.
ClojureScript のリストは、各ノードに値とリストの残りの部分へのポインタが含まれる連結リストだと考えることができます。リストの最後に項目を追加するにはリスト全体を横断する必要がありますが、リストの先頭に項目を追加するのは自然に(高速に)できます。リストの最初に要素を追加するには、@<code>{cons} 関数を用います。

//emlist{
(cons 0 (cons 1 (cons 2 ())))
;; => (0 1 2)
//}

#@# We used the literal () to represent the empty list. Since it doesn't contain any symbols, it is not treated as a function call. However, when using list literals that contain elements, we need to quote them to prevent ClojureScript from evaluating them as a function call:
リストのリテラル表現として @<code>{()} を使いました。シンボルを 1 つも含んでいないので、関数の呼び出しとしては見なされません。もし要素がある場合には、式が評価されないようにクオート @<code>{'} をつける必要があります。

//emlist{
(cons 0 '(1 2))
;; => (0 1 2)
//}

#@# Since the head is the position that has constant time addition in the list collection, the conj operation on lists naturally adds items to the front:
リストコレクションにおいては先頭部は一定の時間で追加できる場所なので、リストに対する @<code>{conj} 操作は要素を自然に先頭部に追加します。

//emlist{
(conj '(1 2) 0)
;; => (0 1 2)
//}

#@# Lists and other ClojureScript data structures can be used as stacks using the peek, pop, and conj functions. Note that the top of the stack will be the "place" where conj adds elements, making conj equivalent to the stack's push operation. In the case of lists, conj adds elements to the front of the list, peek returns the first element of the list, and pop returns a list with all the elements but the first one.
リストと ClojureScript の他のデータ構造において、@<code>{peek}, @<code>{pop}, @<code>{conj} 関数はスタック操作のために使えます。スタックの最上部は @<code>{conj} が要素を追加する場所であることから、@<code>{conj} 操作はスタックへのプッシュ操作と同等です。リストの場合、@<code>{conj} は要素をリストの先頭に追加して、@<code>{peek} はリストの最初の要素を返し、@<code>{pop} は先頭の要素以外の全ての要素を返します。

#@# Note that the two operations that return a stack (conj and pop) don't change the type of the collection used for the stack.
スタックを操作する @<code>{conj} と @<code>{pop} がスタックのために使われるコレクションの型に変更を加えないことに注意してください。


#@# Page38
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def list-stack '(0 1 2))

(peek list-stack)
;; => 0

(pop list-stack)
;; => (1 2)

(type (pop list-stack))
;; => cljs.core/List

(conj list-stack -1)
;; => (-1 0 1 2)

(type (conj list-stack -1))
;; => cljs.core/List
//}

#@# One thing that lists are not particularly good at is random indexed access. Since they are stored in a single linked list-like structure in memory, random access to a given index requires a linear traversal in order to either retrieve the requested item or throw an index out of bounds error. Non-indexed ordered collections like lazy sequences also suffer from this limitation.
リストが特に不得意なのは、インデックスによるランダムアクセスです。リストはメモリ内で連結されたリストのような構造体に格納されるので、要求されたアイテムを取り出すためにも、またインデックスが有効範囲にない場合にエラーを投げるためにも、ランダムに要素にアクセスするには先頭から順に検索する必要があります。遅延シーケンスのようなインデックス付けされていない順序付きコレクションも、この制限を受けます。

===== ベクタ

#@# Vectors

#@# Vectors are one of the most common data structures in  ClojureScript . They are used as a syntactic construct in many places where more traditional  Lisp s use lists, for example in function argument declarations and let bindings.
ベクタは ClojureScript で最も一般的なデータ構造の 1 つです。ベクタは、伝統的な Lisp 方言がリストを使用する多くの場所、例えば、関数の引数宣言や @<code>{let} 束縛での構文内で使われます。

#@# ClojureScript vectors have enclosing brackets [] in their syntax literals. They can be created with vector and from another collection with vec:
ClojureScript のベクタは、リテラル表現として角括弧 @<code>{[]} を使います。@<code>{vector} 関数で生成することも可能です。他のコレクションからベクタを作成するには @<code>{vec} 関数を使います。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(vector? [0 1 2])
;; => true

(vector 0 1 2)
;; => [0 1 2]

(vec '(0 1 2))
;; => [0 1 2]
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Vectors are, like lists, ordered collections of heterogeneous values. Unlike lists, vectors grow naturally from the tail, so the conj operation appends items to the end of a vector. Insertion on the end of a vector is effectively constant time:
ベクタは様々な種類の値が順に並べられたコレクションです。リストとは異なり、ベクタは末尾から自然に拡張することができ、@<code>{conj} 関数はアイテムをベクタの末尾に追加します。ベクタの末尾へ要素を追加する操作は、実質的に一定時間で行えます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(conj [0 1] 2)
;; => [0 1 2]
//}

#@# Page39
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 

#@# Another thing that differentiates lists and vectors is that vectors are indexed collections and as such support efficient random index access and non-destructive updates. We can use the nth function to retrieve values given an index:
リストとベクタを区別するもう 1 つの点は、ベクタはインデックス付きコレクションであり、効率的なランダムアクセスや非破壊更新をサポートする点です。@<code>{nth} 関数を使うと、指定されたインデックスの値を取得できます。

//emlist{
(nth [0 1 2] 0)
;; => 0
//}
 
#@# Since vectors associate sequential numeric keys (indexes) to values, we can treat them as an associative data structure. ClojureScript provides the assoc function that, given an associative data structure and a set of key-value pairs, yields a new data structure with the values corresponding to the keys modified. Indexes begin at zero for the first element in a vector.
ベクタは連続した数値キー(インデックス)を値に関連付けるので、連想データ構造として扱うことができます。ClojureScript が提供する @<code>{assoc} 関数は、連想データの構造体とキーと値のペアのセットを指定すると、変更されたキーに対応する値を持つ新しいデータの構造体を生成します。インデックスは、ベクタの最初の要素の 0 から始まります。

//emlist{
(assoc ["cero" "uno" "two"] 2 "dos")
;; => ["cero" "uno" "dos"]
//}

#@# Note that we can only assoc to a key that is either contained in the vector already or if it is the last position in a vector:
ベクタにすでに含まれているキー、またはベクタの最後の位置にあるキーにのみ @<code>{assoc} を使うことができます。

//emlist{
(assoc ["cero" "uno" "dos"] 3 "tres")
;; => ["cero" "uno" "dos" "tres"]

(assoc ["cero" "uno" "dos"] 4 "cuatro")
;; Error: Index 4 out of bounds [0,3]
//}

#@# Perhaps surprisingly, associative data structures can also be used as functions. They are functions of their keys to the values they are associated with. In the case of vectors, if the given key is not present an exception is thrown:
連想データ構造は関数として使うこともできます。これらは、関連付けられている値に対するキーの関数です。ベクタの場合、指定されたキーが存在しないと例外が発生します。

//emlist{
(["cero" "uno" "dos"] 0)
;; => "cero"

(["cero" "uno" "dos"] 2)
;; => "dos"

(["cero" "uno" "dos"] 3)
;; Error: Not item 3 in vector of length 3
//}

#@# As with lists, vectors can also be used as stacks with the peek, pop, and conj functions. Note, however, that vectors grow from the opposite end of the collection as lists:
ベクタはリストと同様にスタックとして使うことが可能であり、@<code>{peek}, @<code>{pop}, @<code>{conj} を使うことができます。ベクタとリストは要素を追加するときに、先頭と末尾が逆であることに注意してください。


#@# Page40
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def vector-stack [0 1 2])

(peek vector-stack)
;; => 2 

(pop vector-stack)
;; => [0 1]

(type (pop vector-stack))
;; => cljs.core/PersistentVector

(conj vector-stack 3)
;; => [0 1 2 3]

(type (conj vector-stack 3))
;; => cljs.core/PersistentVector
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# The map and filter operations return lazy sequences, but as it is common to need a fully realized sequence after performing those operations, vector-returning counterparts of such functions are available as mapv and filterv. They have the advantages of being faster than building a vector from a lazy sequence and making your intent more explicit:
@<code>{map} と @<code>{filter} の操作は遅延シーケンスを返しますが、これらの操作の後には完全に実現されたシーケンスが必要なことが一般的なため、ベクタを返す同等のものとして @<code>{mapv} や @<code>{filterv} 等があります。これらの関数には、遅延シーケンスからベクタを構築するよりも高速であり、意図をより明確にするという利点があります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(map inc [0 1 2])
;; => (1 2 3)

(type (map inc [0 1 2]))
;; => cljs.core/LazySeq

(mapv inc [0 1 2])
;; => [1 2 3]

(type (mapv inc [0 1 2]))
;; => cljs.core/PersistentVector
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

===== マップ

#@# Maps

#@# Maps are ubiquitous in  ClojureScript. Like vectors, they are also used as a syntactic construct, particularly for attaching metadata to  vars. Any ClojureScript data structure can be used as a key in a map, although it's common to use keywords since they can also be called as functions.
#@# ClojureScript maps are written literally as key-value pairs enclosed in braces `{}`. Alternatively, they can be created with the hash-map function:
マップは ClojureScript で広く使われています。ベクタと同様に、マップはメタデータを vars に付与するために使われます。ClojureScript のデータ構造体は全てマップのキーとして使えますが、キーワードは関数としても呼び出すことができるため、キーワードを使うことが一般的です。
ClojureScript のマップは、リテラル表現として、キーと値のペアを中括弧 @<code>${ }$ で囲んで記述します。代わりに @<code>{hash-map} 関数で生成することもできます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(map? {:name "Cirilla"})
;; => true

(hash-map :name "Cirilla")
;; => {:name "Cirilla"}
//}

 
#@# Page41
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}
 

//emlist{ 
(hash-map :name "Cirilla" :surname "Fiona")
;; => {:name "Cirilla" :surname "Fiona"}
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# Since regular maps don't have a specific order, the conj operation just adds one or more key-value pairs to a map. conj for maps expects one or more sequences of key-value pairs as its last arguments:
通常のマップには特定の順番がないため、@<code>{conj} の操作は 1 つ以上のキーと値のペアをマップに追加するだけです。@<code>{conj} をマップに使うには、最後の引数として 1 つ以上のキーと値のペアのシーケンスを想定しています。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(def ciri {:name "Cirilla"})

(conj ciri [:surname "Fiona"])
;; => {:name "Cirilla", :surname "Fiona"}

(conj ciri [:surname "Fiona"] [:occupation "Wizard"])
;; => {:name "Cirilla", :surname "Fiona", :occupation "Wizard"}
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}} 
 
#@# In the preceding example, it just so happens that the order was preserved, but if you have many keys, you will see that the order is not preserved.
#@# Maps associate keys to values and, as such, are an associative data structure. They support adding associations with assoc and, unlike vectors, removing them with dissoc. assoc will also update the value of an existing key.  let 's explore these functions:
上の例では要素の順が保たれていますが、もし多くのキーを含む場合、順序が保持されていないことがわかります。
マップはキーと値を関連付ける連想型のデータ構造です。マップに要素を追加するには @<code>{assoc} を使い、マップから要素を取り除くには @<code>{dissoc} を使います。@<code>{assoc} はすでにあるキーの値を更新することができます。これらの関数を試してみましょう。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
(assoc {:name "Cirilla"} :surname "Fiona")
;; => {:name "Cirilla", :surname "Fiona"}

(assoc {:name "Cirilla"} :name "Alfonso")
;; => {:name "Alfonso"}

(dissoc {:name "Cirilla"} :name)
;; => {}
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}} 
 
#@# Maps are also functions of their keys, returning the values related to the given keys. Unlike vectors, they return nil if we supply a key that is not present in the map:
マップはキーの関数でもあり、指定されたキーに関連する値を返します。ベクタとは異なり、マップに存在しないキーを指定すると、@<code>{nil} が返されます。

@<embed>{|latex|\vspace{-0.2\Cvs\}}

//emlist{
({:name "Cirilla"} :name)
;; => "Cirilla"

({:name "Cirilla"} :surname)
;; => nil
//}

@<embed>{|latex|\vspace{-0.2\Cvs\}}

#@# ClojureScript also offers sorted hash maps which behave like their unsorted versions but preserve order when iterating over them. We can create a sorted map with default ordering with sorted-map:
ClojureScript はソートされたハッシュマップも提供しています。ソートされていないバージョンと同じように動作しますが、繰り返しの際に順序を保持します。@<code>{sorted-map} を使用すると、デフォルトの順序でソートされたマップを作成できます。

#@# Page42
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def sm (sorted-map :c 2 :b 1 :a 0))
;; => {:a 0, :b 1 , :c 2 }

(keys sm)
;; => (:a :b :c)
//}

#@# If we need a custom ordering we can provide a comparator function to sorted-map-by,  let 's see an example inverting the value returned by the built-in compare function. Comparator functions take two items to compare and return - 1 (if the first item is less than the second), 0 (if they are equal), or 1 (if the first item is greater than the second).
独自の順序付けが必要な場合は、@<code>{sorted-map-by} に比較関数を与えられます。組み込みの @<code>{compare} 関数から返される値を反転する例を見てみましょう。@<code>{compare} 関数は 2 つの要素を比較して、最初の項目が 2 番目の項目より小さい場合は -1 、等しい場合は 0 、最初の項目が 2番目の項目よりも大きい場合 1 を返します。

//emlist{
(defn reverse-compare [a b] (compare b a))

(def sm (sorted-map-by reverse-compare :a 0 :b 1 :c 2))
;; => {:c 2 , :b 1 , :a 0}

(keys sm)
;; => (:c :b :a)
//}

===== セット

#@# Sets

#@# Sets in ClojureScript have literal syntax as values enclosed in `#{}` and they can be created with the set constructor. They are unordered collections of values without duplicates.
ClojureScript のセットは @<code>$#{}$ のリテラル表現を用いて作成されます。@<code>{set} コンストラクタを用いて作成することもできます。セットは、要素間の順番がないコレクションであり、同じ要素の重複は許されません。

//emlist{
(set? #{\a \e \i \o \u})
;; => true

(set [1 1 2 3])
;; => #{1 2 3}
//}

#@# Set literals cannot contain duplicate values. If you accidentally write a set literal with duplicates an error will be thrown:
セットのリテラル表現は同じ値を重複して含むことができません。もし要素が重複する場合、エラーが発生します。

//emlist{
#{1 1 2 3}
;; clojure.lang.ExceptionInfo: Duplicate key: 1 
//}

#@# There are many operations that can be performed with sets, although they are located in the `clojure.set` namespace and thus need to be imported. You'll learn the details of namespacing later; for now, you only need to know that we are loading a namespace called `clojure.set` and binding it to the s symbol.
セットを操作するための関数は多くあります。@<code>{clojure.set} の名前空間にあるため、インポートする必要があります。名前空間については後に詳しく学びます。現時点では、@<code>{clojure.set} の名前空間をインポート後、シンボル @<code>{s} に束縛していることが分かれば十分です。


#@# Page43
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[clojure.set :as s])

(def danish-vowels #{\a \e \i \o \u \æ \ø \å})
;; => #{"a" "e" "å" "æ" "i" "o" "u" "ø"}

(def spanish-vowels #{\a \e \i \o \u})
;; => #{"a" "e" "i" "o" "u"}

(s/difference danish-vowels spanish-vowels)
;; => #{"å" "æ" "ø"}

(s/union danish-vowels spanish-vowels)
;; => #{"a" "e" "å" "æ" "i" "o" "u" "ø"}

(s/intersection danish-vowels spanish-vowels)
;; => #{"a" "e" "i" "o" "u"}
//}

//embed[latex]{
\vspace{-0.4\Cvs}
//}
 
#@# A nice property of immutable sets is that they can be nested. Languages that have mutable sets can end up containing duplicate values, but that can't happen in  ClojureScript . In fact, all ClojureScript data structures can be nested arbitrarily due to immutability.
#@# Sets also support the generic conj operation just like every other collection does.

不変であるセットの優れた特性はネストできることです。可変であるセットをもつプログラミング言語では重複した値を含みますが、ClojureSript では重複を許しません。実際、全ての ClojureScript のデータ構造は不変性の性質のおかげで任意にネストすることができます。セットは他のコレクションのデータ型と同様に @<code>{conj} を使うことも可能です。

//embed[latex]{
\vspace{-0.4\Cvs}
//}

//emlist{
(def spanish-vowels #{\a \e \i \o \u})
;; => #{"a" "e" "i" "o" "u"}

(def danish-vowels (conj spanish-vowels \æ \ø \å))
;; => #{"a" "e" "i" "o" "u" "æ" "ø" "å"}

(conj #{1 2 3} 1)
;; => #{1 3 2}
//}

//embed[latex]{
\vspace{-0.4\Cvs}
//}
 
#@# Sets act as read-only associative data that associates the values it contains to themselves. Since every value except nil and false is truthy in  ClojureScript , we can use sets as predicate functions:
セットは、読み取り専用の連想型データとして動作して、セットに含まれる値をセット自体に関連づけます。ClojureScript では @<code>{nil} と @<code>{false} 以外の全ての値が真であるため、述語関数としてセットを使用できます。

//embed[latex]{
\vspace{-0.4\Cvs}
//}

//emlist{
(def vowels #{\a \e \i \o \u})
;; => #{"a" "e" "i" "o" "u"}

(get vowels \b)
;; => nil

(contains? vowels \b)
;; => false

(vowels \a)
;; => "a"
//}


#@# Page44
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}
 

//emlist{ 
(vowels \z)
;; => nil

(filter vowels "Hound dog")
;; => ("o" "u" "o")
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Sets have a sorted counterpart like maps do that are created using the functions sorted-set and sorted-set-by which are analogous to map's sorted-map and sorted-map-by.
セットにもマップのようにソート版があり、マップの @<code>{sorted-map} と @<code>{sorted-map-by} に相当する @<code>{sorted-set} と @<code>{sorted-set-by} 関数で作成されます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def unordered-set #{[0] [1] [2]})
;; => #{[0] [2] [1]}

(seq unordered-set)
;; => ([0] [2] [1])

(def ordered-set (sorted-set [0] [1] [2]))
;; =># {[0] [1] [2]}

(seq ordered-set)
;; => ([0] [1] [2])
//}

===== キュー

#@# Queues

#@# ClojureScript also provides a persistent and immutable queue. Queues are not used as pervasively as other collection types.  They can be created using the `#queue []` literal syntax, but there are no convenient constructor functions for them.
さらに ClojureScript は、永続的で不変であるキューを提供します。キューは他のコレクションのように広くは使われていません。キューは @<code>{#queue []} というリテラル構文で作ることができますが、便利なコンストラクタ関数はありません。

//emlist{
(def pq #queue [1 2 3])
;; => #queue [1 2 3]
//}

#@# Using conj to add values to a queue adds items onto the rear:
値をキューに追加するために conj を使うと、アイテムは末尾に追加されます。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def pq #queue [1 2 3])
;; => #queue [1 2 3]

(conj pq 4 5)
;; => #queue [1 2 3 4 5]
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# A thing to bear in mind about queues is that the stack operations don't follow the usual stack semantics (pushing and popping from the same end). pop takes values from the front position, and conj pushes (appends) elements to the back.
キューに関して留意すべき点は、スタック操作が通常のスタックのセマンティクス(同じ終点から出し入れすること)に従わないことです。@<code>{pop} は前の位置から値を取り、@<code>{conj} は要素を後ろに push(もしくは append) します。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page45
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def pq #queue [1 2 3])
;; => #queue [1 2 3]

(peek pq)
;; => 1 

(pop pq)
;; => #queue [2 3]

(conj pq 4)
;; => #queue [1 2 3 4]
//}

#@# Queues are not as frequently used as lists or vectors, but it is good to know that they are available in  ClojureScript , as they may occasionally come in handy.
キューはリストやベクタほど頻繁には使われませんが、ClojureScript では手軽に利用できることを知っておくと便利です。

== Destructuring

#@# Destructuring

#@# Destructuring, as its name suggests, is a way of taking apart structured data such as collections and focusing on individual parts of them. ClojureScript offers a concise syntax for destructuring both indexed sequences and associative data structures that can be used any place where bindings are declared.
destructuring（非構造化）という言葉が示すように、destructuring はコレクションのような構造化されたデータを分解して、個別の要素に焦点を当てる方法です。ClojureScript は分割のための構文はシンプルです。destructuring は、インデックスがついたシーケンスに対しても、連想型のデータ構造に対しても、束縛が宣言された場所であればどこでも使うことができます。

#@#  let 's see an example of what destructuring is useful for that will help us understand the previous statements better. Imagine that you have a sequence but are only interested in the first and third item. You could get a reference to them easily with the nth function:
前の説明を理解するために、destructuring の使用例を見てみましょう。シーケンスがあり、1 番目と 3 番目の要素だけに関心があるとします。これらへの参照は、@<code>{nth} 関数で簡単に取得できます。

//emlist{
(let [v [0 1 2]
      fst (nth v 0)
      thrd (nth v 2)]
  [thrd fst])
;; => [2 0]
//}

#@# However, the previous code is overly verbose. Destructuring  let s us extract values of indexed sequences more succintly using a vector on the left-hand side of a binding:
しかし、前のコードは冗長すぎます。destructuring では、束縛の左側のベクタを使用して、インデックスがついたシーケンスからよりシンプルに値を取り出すことができます。

//emlist{
(let [[fst _ thrd] [0 1 2]]
  [thrd fst])
;; => [2 0]
//}

#@# In the above example, `[fst _ thrd]` is a destructuring form. It is represented as a vector and used for binding indexed values to the symbols fst and thrd, corresponding to the index 0 and 2 , respectively. The _ symbol is used as a placeholder for indexes we are not interested in — in this case 1 .
上の例では、@<code>{[fst _ thrd]} がdestructuring の箇所です。これはベクタとして表され、@<code>{fst} と @<code>{thrd} に対応するインデックス @<code>{0} と @<code>{2} の値がそれぞれ束縛されます。シンボルの @<code>{_} は、興味のない値(この場合は @<code>{1})のプレースホルダとして使われます。

#@# Page46
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# Note that destructuring is not limited to the let binding form; it works in almost every place where we bind values to symbols such as in the for and doseq special forms or in function arguments. We can write a function that takes a pair and swaps its positions very concisely using destructuring syntax in function arguments:
destructuring は @<code>{let} での束縛に限定されないことに注意してください。@<code>{for} や @<code>{doseq} の特殊形式、また関数の引数部など、シンボルに値を束縛する全ての場所で動作します。destructuring 構文を関数の引数部に使うことで、ペアをとり、その位置をスワップする関数を作成できます。

//emlist{
(defn swap-pair [[fst snd]]
  [snd fst])

(swap-pair [1 2])
;; => [2 1]

(swap-pair '(3 4))
;; => [4 3]
//}

#@# Positional destructuring with vectors is quite handy for taking indexed values out of sequences, but sometimes we don't want to discard the rest of the elements in the sequence when destructuring.  Similarly to how & is used for accepting  variadic function arguments, the ampersand can be used inside a vector destructuring form for grouping together the rest of a sequence:
ベクタを用いた位置の destructuring は、シーケンスからインデックス付きの値を取り出すために非常に便利ですが、シーケンス内の残りの要素を破棄したくない場合があります。可変長引数関数の引数を @<code>{&} で受け取る方法と同様に、@<code>{&} をベクタの destructuring 内で使用して、シーケンスの残りの部分をグループ化することができます。

//emlist{
(let [[fst snd & more] (range 10)]
  {:first fst
   :snd snd
   :rest more})
;; => {:first 0, :snd 1, :rest (2 3 4 5 6 7 8 9)}
//}

#@# Notice how the value in the 0 index got bound to fst, the value in the 1 index got bound to snd, and the sequence of elements from 2 onwards got bound to the more symbol.
@<code>{0} のインデックスの値が @<code>{fst} に束縛され、@<code>{1} のインデックスの値が @<code>{snd} に束縛され、@<code>{2} 以降のインデックスのシーケンスがシンボル @<code>{more} に束縛されていることに注目してください。

#@# We may still be interested in a data structure as a whole even when we are destructuring it. This can be achieved with the :as keyword. If used inside a destructuring form, the original data structure is bound to the symbol following that keyword:
データ構造を destructutring する際に、データ構造全体に興味がある場合があります。これには @<code>{:as} キーワードを使うことで、任意の名前でデータ構造をシンボルに束縛することができます。destructuring 内で使用される場合、元のデータ構造は @<code>{:as} の後のシンボルに束縛されます。

//emlist{
(let [[fst snd & more :as original] (range 10)]
  {:first fst
   :snd snd
   :rest more
   :original original})
;; => {:first 0, :snd 1,
;;     :rest (2 3 4 5 6 7 8 9),
;;     :original (0 1 2 3 4 5 6 7 8 9)}
//}

#@# Not only can indexed sequences be destructured, but associative data can also be destructured. Its destructuring binding form is represented as a map instead of a vector, where the keys are the symbols we want to bind values to and the values are the keys that we want to look up in the associative data structure.  let 's see an example:
インデックス付きのシーケンスだけでなく、連想型のデータも destructuring することができますが、ベクタではなくマップを用いて束縛を行います。キーは値を束縛するためのシンボルであり、値は連想型データ構造で検索するキーです。例を見てみましょう。


#@# Page47
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(let [{language :language} {:language "ClojureScript"}]
  language)
;; => "ClojureScript"
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# In the above example, we are extracting the value associated with the :language key and binding it to the language symbol. When looking up keys that are not present, the symbol will get bound to nil:
上の例では、@<code>{:language} キーと関連づけられた値を取り出して、@<code>{language} シンボルに束縛しています。もし探しているキーが存在しない場合は @<code>{nil} が束縛されます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [{name :name} {:language "ClojureScript"}]
  name)
;; => nil
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Associative destructuring lets us give default values to bindings which will be used if the key isn't found in the data structure we are taking apart. A map following the :or keyword is used for default values as the following examples show:
連想型での destructuring では、束縛にデフォルト値を指定できます。このデフォルト値は、destructuring　を行うデータ構造でキーが見つからない場合に使用されます。@<code>{:or} キーワードをマップの後につけることで、デフォルトの値が束縛されます。次の例を見てください。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [{name :name :or {name "Anonymous"}}
      {:language "ClojureScript"}]
  name)
;; => "Anonymous"

(let [{name :name :or {name "Anonymous"}} {:name "Cirilla"}]
  name)
;; => "Cirilla"
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Associative destructuring also supports binding the original data structure to a symbol placed after the :as keyword:
連想型の destructuring では、元のデータ構造を @<code>{:as} キーワードの後に置かれたシンボルに束縛することも可能です。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [{name :name :as person} {:name "Cirilla" :age 49}]
  [name person])
;; => ["Cirilla" {:name "Cirilla" :age 49}]
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Keywords aren't the only things that can be the keys of associative data structures. Numbers, strings, symbols and many other data structures can be used as keys, so we can destructure using those, too. Note that we need to quote the symbols to prevent them from being resolved as a var lookup:
連想型データ構造のキーとして使えるのは、キーワードだけではありません。数字、文字列、シンボル、およびその他の多くのデータ構造もキーとして使えるため、これらを使用して構造を変更することもできます。シンボルが var 検索として解決されないようにするには、シンボルをクオートする必要があります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(let [{one 1} {0 "zero" 1 "one"}]
  one)
;; => "one"
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}
 
#@# Page48
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(let [{name "name"} {"name" "Cirilla"}]
  name)
;; => "Cirilla"

(let [{lang 'language} {'language "ClojureScript"}]
  lang)
;; => "ClojureScript"
//}

#@# Since the values corresponding to keys are usually bound to their equivalent symbol representation (for example, when binding the value of :language to the symbol language) and keys are usually keywords, strings, or symbols, ClojureScript offers shorthand syntax for these cases.
キーに対応する値はシンボルと等しいことが多く(例えば @<code>{:language} の値を @<code>{language} シンボルに束縛する等)、キーはキーワード、文字列、シンボルであることが多いため、ClojureScript にはこれらのために略記法があります。

#@# We'll show examples of all of these, starting with destructuring keywords using :keys:
@<code>{:keys} を用いて キーワードの destructuring を始めます。以下に全ての例を示します。

//emlist{
(let [{:keys [name surname]} {:name "Cirilla" :surname "Fiona"}]
  [name surname])
;; => ["Cirilla" "Fiona"]
//}

#@# As you can see in the example, if we use the :keys keyword and associate it with a vector of symbols in a binding form, the values corresponding to the keywordized version of the symbols will be bound to them. The `{:keys [name surname]}` destructuring is equivalent to `{name :name surname :surname}`, only shorter.
#@# The string and symbol shorthand syntax works exactly like :keys, but using the :strs and :syms keywords respectively:
例の通り、@<code>{:keys} を用いて束縛内でシンボルのベクタと関連づけた場合、シンボルのキーワードに対応する値はそれらに対応づけられます。つまり、@<code>${:keys [name surname]}$ は @<code>${name :name surname :surname}$ の略記法であり、同じ意味です。

//emlist{
(let [{:strs [name surname]} {"name" "Cirilla" "surname" "Fiona"}]
  [name surname])
;; => ["Cirilla" "Fiona"]

(let [{:syms [name surname]} {'name "Cirilla" 'surname "Fiona"}]
  [name surname])
;; => ["Cirilla" "Fiona"]
//}

#@# An interesting property of destructuring is that we can nest destructuring forms arbitrarily, which makes code that accesses nested data on a collection very easy to understand, as it mimics the collection's structure:
また、destructuring の興味深い特性は、任意で destructuring をネストできることです。これにより、コレクション上のネストされたデータにアクセスするコードは、コレクションの構造を模倣するため、非常に理解しやすくなります。

//emlist{
(let [{[fst snd] :languages} {:languages ["ClojureScript" "Clojure"]}]
  [snd fst])
;; => ["Clojure" "ClojureScript"]
//}


#@# Page49
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


== Threading マクロ

#@# Threading Macros

#@# Threading macros, also known as arrow functions, enables one to write more readable code when multiple nested function calls are performed.
Threading マクロはアロー関数として知られています。Threading マクロを使うことで、ネストされた複数の関数呼び出しを実行するときに、読みやすいコードを記述できます。

#@# Imagine you have `(f (g (h x)))` where a function f receives as its first parameter the result of executing function g, repeated multiple times. With the most basic `->` threading macro you can convert that into `(\-> x (h) (g) (f))` which is easier to read.
例えば @<code>{(f (g (h x)))} というコードにおいて、@<code>{f} は @<code>{g} の結果を第 1 引数として受け取ります。Threading マクロ @<code>{->} を使うことで、@<code>{(-> x (h) (g) (f))} と書くことができます。こちらのほうが読みやすいですね。

#@# The result is syntactic sugar, because the arrow functions are defined as macros and it does not imply any runtime performance. The `(\-> x (h) (g) (f))` is automatically converted to (f (g (h x))) at compile time.
Threading マクロはマクロとして定義されているため、実行時のパフォーマンスに影響を与えません。@<code>{(-> x (h) (g) (f))} はコンパイル時に @<code>{(f (g (h x)))} に変換されます。

#@# Take note that the parenthesis on h, g and f are optional, and can be ommited: `(f (g (h x)))` is the same as `(\-> x h g f)`.
@<code>{h}, @<code>{g}, @<code>{f} の括弧はなくてもかまいません。@<code>{(f (g (h x)))} は @<code>{(-> x h g f)} と省略して書くこともできます。

=== thread-first マクロ ( @<code>{->} )

#@# This is called *thread first* because it threads the first argument throught the different expressions as first arguments.
@<code>{->} は thread-firstマクロ と呼ばれますが、複数の式が実行されていく中で、最初の引数に値を挿入していくことから、そのような名前がつけられています。

#@# Using a more concrete example, this is how the code looks without using threading macros:
では具体例として、まずは Threading マクロを使わない場合を見てみましょう。

//emlist{
(def book {:name "Lady of the Lake"
           :readers 0})

(update (assoc book :age 1999) :readers inc)
;; => {:name "Lady of the lake" :age 1999 :readers 1}
//}

#@# We can rewrite that code to use the `\->` threading macro:
thread-firstマクロ (@<code>{->}) を使うと、次のように書くことができます。

//emlist{
(-> book
    (assoc :age 1999)
    (update :readers inc))
;; => {:name "Lady of the lake" :age 1 999 :readers 1}
//}

#@# This threading macro is especially useful for transforming data structures, because ClojureScript (and _Clojure_) functions for data structures transformations consistently uses the first argument for receive the data structure.
このリーダマクロはデータ構造を変換するときに特に役にたちます。これは ClojureScript (Clojure) のデータ構造を変換する関数は、一貫して最初の引数でデータ構造を受け取るからです。

=== thread-last マクロ ( @<code>{->>} )

#@# The main difference between the thread-last and thread-first macros is that instead of threading the first argument given as the first argument on the following expresions, it threads it as the last argument.
#@#  let 's look at an example:
thread-last マクロ (@<code>{->>}) と thread-first マクロ (@<code>{->}) の主な違いは、最初の引数にデータを入れるのではなく、最後の引数にデータを入れる点です。例を見てみましょう。


#@# Page50
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{
(def numbers [1 2 3 4 5 6 7 8 9 0])

(take 2 (filter odd? (map inc numbers)))
;; => (3 5)
//}

#@# The same code written using `\->>` threading macro:
上の例は thread-last マクロ (@<code>{->>}) を用いて、次のように書き直すことができます。

//emlist{
(->> numbers
     (map inc)
     (filter odd?)
     (take 2))
;; => (3 5)
//}

#@# This threading macro is especially useful for transforming sequences or collections of data because ClojureScript functions that work with sequences and collections consistently use the last argument position to receive them.
thread-last マクロ (@<code>{->>}) は、シーケンスのデータを変形するときに役にたちます。ClojureScript のシーケンスとコレクションを操作する関数は、シーケンスやコレクションを最後の引数として受け取るためです。

=== thread-as マクロ ( @<code>{as->} )

#@# Finally, there are cases where neither `\->` nor `\->>` are applicable. In these cases, you’ll need to use `as\->`, the more flexible alternative, that allows you to thread into any argument position, not just the first or last.
最後に @<code>{->} と @<code>{->>} どちらにも適用できない場合があります。この場合は、@<code>{as->} マクロを使います。@<code>{as->} マクロは、最初と最後に限らず、好きな場所にデータを入れることができます。

#@# It expects two fixed arguments and an arbitrary number of expressions. As with `\->`, the first argument is a value to be threaded through the following forms. The second argument is the name of a binding. In each of the subsequent forms, the bound name can be used for the prior expression's result.
@<code>{as->} マクロは 2 つの引数を固定でもち、その後に任意の数の式が続きます。@<code>{->} と同様に、最初の引数は以降の式に渡されていく値です。2 番目の引数は束縛の名前です。以降の各々の式で、前の式の結果のためにその束縛の名前が使われます。

#@#  let 's see an example:
例を見てみましょう。

//emlist{
(as-> numbers $
  (map inc $)
  (filter odd? $)
  (first $)
  (hash-map :result $ :id 1))
;; => {:result 3 :id 1}
//}


=== thread-some マクロ ( @<code>{some->} と @<code>{some->>} )

#@# Two of the more specialized threading macros that ClojureScript comes with. They work in the same way as their analagous `\->` and `\->>` macros with the additional support for short-circuiting the expression if one of the expresions evaluates　to nil.
ClojureScript のより特殊なスレッド系マクロを 2 つ紹介します。これらは @<code>{->} と @<code>{->>} と同じように動作します。式の 1 つが @<code>{nil} と評価された場合に式を短絡することをサポートします。

#@#  let 's see another example:
例を見てみましょう。


#@# Page51
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{
(some-> (rand-nth [1 nil])
        (inc))
;; => 2 

(some-> (rand-nth [1 nil])
        (inc))
;; => nil
//}

#@# This is an easy way avoid null pointer exceptions.
これにより、ヌル・ポインタ・エクセプションを簡単に回避することができます。

#@# ;; 注釈
#@# ;; user=> (inc nil)
#@# ;; Execution error (NullPointerException) at user/eval241 (REPL:1).
#@# ;; null

=== thread-cond マクロ ( @<code>{cond->} と @<code>{cond->>} )

#@# The `cond\->` and `cond\->>` macros are analgous to `\->` and `\->>` that offers the ability to conditionally skip some steps from the pipeline. let see an example:
@<code>{cond->} と @<code>{cond->>} マクロは @<code>{->} と @<code>{->>} と似ており、パイプラインからの処理を条件によりスキップできます。例を見てみましょう。

//emlist{
(defn describe-number
  [n]
  (cond-> []
    (odd? n) (conj "odd")
    (even? n) (conj "even")
    (zero? n) (conj "zero")
    (pos? n) (conj "positive")))

(describe-number 3)
;; => ["odd" "positive"]

(describe-number 4)
;; => ["even" "positive"]
//}

#@# The value threading only happens when the corresponding condition evaluates to logical true.
対応する条件が論理的に真と評価された場合のみ、値が引き渡されます。

=== 追加資料

詳しい情報は次の文献を参照してください。

- Lesser known Clojure: variants of threading macro@<fn>{thm1}

- Threading Macros Guide@<fn>{thm2}

//footnote[thm1][http://www.spacjer.com/blog/2015/11/09/lesser-known-clojure-variants-of-threading-macro]
//footnote[thm2][http://clojure.org/guides/threading_macros]


#@# Page52
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 


== リーダーコンディショナル (Reader Conditionals)

#@# This language feature allows different dialects of Clojure to share common code that is mostly platform independent but need some platform dependent code.
この言語の特徴として、Clojure の異なる方言ごとに、コードを共有したり、プラットフォームに依存するコードを書くことができます。

#@# To use reader conditionals, all you need is to rename your source file with `.cljs` extension to one with `.cljc`, because reader conditionals only work if they are placed in files with `.cljc` extension.
リーダーコンディショナル(Reader Conditionals)を使うには、ファイルの拡張子を @<code>{.cljs} から @<code>{.cljc} に変更する必要があります。リーダの条件分けは拡張子が @<code>{.cljc} の場合にのみ動作します。

=== Standard (@<code>{#?})

#@# Standard (`#?`)

#@# There are two types of reader conditionals, standard and splicing. The standard reader conditional behaves similarly to a traditional cond and the syntax looks like this:
リーダーコンディショナルには、standard と splicing の 2 タイプがあります。standard の場合は、@<code>{cond} のように書くことができます。

//emlist{
(defn parse-int
  [v]
  #?(:clj  (Integer/parseInt v)
     :cljs (js/parseInt v)))
//}

#@# As you can observe, `#?` reading macro looks very similar to cond, the difference is that the condition is just a keyword that identifies the platform, where :cljs is for ClojureScript and :clj is for _Clojure_. The advantage of this approach, is that it is evaluated at compile time so no runtime performance overhead exists for using this.
リーダマクロの @<code>{#?} は @<code>{cond} と同じように見えますが、条件部にキーワードを用いて、ClojureScript には @<code>{:cljs} 、Clojure には @<code>{:clj} をプラットフォームの特定のために使います。リーダマクロはコンパイル時に評価されるので、実行時にオーバーヘッドが発生しません。

=== Splicing ( @<code>{#?@} )

#@# Splicing (`#?@`)

#@# The splicing reader conditional works in the same way as the standard and allows splice lists into the containing form. The `#?@` reader macro is used for that and the code looks like this:
リーダーコンディショナルの splicing は、standard と同じように使いますが、リストを結合(splice)できます。このためにリーダマクロ @<code>{#?@} を使います。

//emlist{
(defn make-list
  []
  (list #?@(:clj  [5 6 7 8]
            :cljs [1 2 3 4])))

;; ClojureScript の場合
(make-list)
;; => (1 2 3 4)
//}
 
@<embed>{|latex|\vspace{-0.3\Cvs\}} 

#@# The ClojureScript compiler will read that code as this:
ClojureScript のコンパイラは、上のコードをこのように解釈します。

@<embed>{|latex|\vspace{-0.3\Cvs\}} 

//emlist{
(defn make-list
  []
  (list 1 2 3 4))
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}} 

 
#@# Page53
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


#@# The splicing reader conditional can't be used to splice multiple top level forms, so the following code is ilegal:
リーダーコンディショナルの splicing では、複数のトップレベルのフォームを結合することはできないため、次のコードは不正です。

//embed[latex]{
\vspace{-0.5\Cvs}
//} 
 
//emlist{
#?@(:cljs [(defn func-a [] :a)
           (defn func-b [] :b)])
;; => #error "Reader conditional splicing not allowed at the top level."
//}

#@# If you need so, you can use multiple forms or just use do block for group multiple forms together:
そのようにするには、複数のフォームを使うか、@<code>{do} ブロックを使う必要があります。

//emlist{
#?(:cljs (defn func-a [] :a))
#?(:cljs (defn func-b [] :b))

;; または、

#?(:cljs
   (do
     (defn func-a [] :a)
     (defn func-b [] :b)))
//}


=== 参考文献

#@# More readings

詳しい内容は以下の文献を参考にしてください。

- Reader Conditionals Guide@<fn>{con1}
 
- Clojure Reader Conditionals by Example@<fn>{con2}
 
- cuerdas - string manipulation library@<fn>{con3}
 
//footnote[con1][http://clojure.org/guides/reader_conditionals]
//footnote[con2][https://danielcompton.net/2015/06/10/clojure-reader-conditionals-by-example]
//footnote[con3][https://github.com/funcool/cuerdas]

 
#@# Page54
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 
  
#@# @<fn>{}
#@# //footnote[][]
 
== 名前空間

#@# Namespaces

=== 名前空間の定義

#@# Defining a namespace

#@# The _namespace_ is  ClojureScript 's fundamental unit of code modularity. Namespaces are analogous to Java packages or Ruby and Python modules and can be defined with the ns macro. If you have ever looked at a little bit of ClojureScript source, you may have noticed something like this at the beginning of the file:
名前空間は、ClojureScript でコードをモジュール化する基本単位です。ClojureScript の名前空間は、Java のパッケージ、Ruby や Python のモジュールに似ており、@<code>{ns} マクロで定義できます。ClojureScript のソースを少し見たことがある人は、ファイルの最初に次のように書かれていることに気付いたかもしれません。

//emlist{
(ns myapp.core
  "Some docstring for the namespace.")

(def x "hello")
//}

#@# Namespaces are dynamic, meaning you can create one at any time. However, the convention is to have one namespace per file. Naturally, a namespace definition is usually at the beginning of the file, followed by an optional docstring.
名前空間は動的であり、いつでも生成することができます。ただし、ファイルごとに 1 つの名前空間を使用するのが規則です。通常、名前空間の定義はファイルの先頭にあり、その後に オプションの docstring が続きます。

#@# Previously we have explained  var s and symbols. Every var that you define will be associated with its namespace. If you do not define a concrete namespace, then the default one called "cljs.user" will be used:
var とシンボルについては前に説明しました。定義する全ての var は、その名前空間に関連づけられます。具体的な名前空間を定義しない場合は、デフォルトの @<code>{cljs.user} が名前空間として使われます。

//emlist{
(def x "hello")
;; => #'cljs.user/x
//}

=== 他の名前空間の読み込み

#@# Loading other namespaces

#@# Defining a namespace and the  vars in it is really easy, but it's not very useful if we can't use symbols from other namespaces. For this purpose, the ns macro offers a simple way to load other namespaces.
#@# Observe the following:
名前空間を定義して、その中で変数を定義するのは非常に簡単ですが、他の名前空間のシンボルが使えなければ便利でありません。この目的のために、@<code>{ns} マクロは他の名前空間をロードする簡単な方法を提供します。

//emlist{
(ns myapp.main
  (:require myapp.core
            clojure.string))

(clojure.string/upper-case myapp.core/x)
;; => "HELLO"
//}

#@# As you can observe, we are using fully qualified names (namespace + var name) for access to  var s and functions from different namespaces.
ご覧のとおり、別の名前空間から var や関数にアクセスするためには、完全な修飾名(名前空間 + 変数名)を使用します。

#@# Page55
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


#@# While this will let you access other namespaces, it's also repetitive and overly verbose. It will be especially uncomfortable if the name of a namespace is very long. To solve that, you can use the :as directive to create an additional (usually shorter) alias to the namespace.  This is how it can be done:
これにより他の名前空間にアクセスできますが、繰り返しが多く、冗長になりすぎます。名前空間の名前が長い場合は特に不便です。これを解決するには、@<code>{:as} ディレクティブを利用して、名前空間に追加の(通常より短い)エイリアスを作成します。その方法は次のとおりです。

//emlist{
(ns myapp.main
  (:require [myapp.core :as core]
            [clojure.string :as str]))

(str/upper-case core/x)
;; => "HELLO"
//}

#@# Additionally, ClojureScript offers a simple way to refer to specific  var s or functions from a concrete namespace using the :refer directive, followed by a sequence of symbols that will refer to  var s in the namespace. Effectively, it is as if those  var s and functions are now part of your namespace, and you do not need to qualify them at all.
さらに ClojureScript では、@<code>{:refer} ディレクティブを用いることで、特定の名前空間から var や関数を参照することが容易になります。@<code>{:refer} の後には、特定の名前空間内の var を参照するシンボルのシーケンスを書きます。事実上、これらの var と関数はあなたの名前空間の一部のようになるため、これらに修飾子をつける必要はありません。

//emlist{
(ns myapp.main
  (:require [clojure.string :refer [upper-case]]))

(upper-case x)
;; => "HELLO"
//}

#@# And finally, you should know that everything located in the `cljs.core` namespace is automatically loaded and you should not require it explicitly. Sometimes you may want to declare  var s that will clash with some others defined in the `cljs.core` namespace. To do this, the ns macro offers another directive that allows you to exclude specific symbols and prevent them from being automatically loaded.
最後に、@<code>{cljs.core} の中にある全てのものを把握する必要があります。@<code>{cljs.core} の名前空間は自動的にロードされるので、明示的に require すべきではありません。@<code>{cljs.core} で定義されている var と衝突する var を宣言する場合、@<code>{ns} マクロは特定のシンボルを除外して自動的にロードされないようにする別のディレクティブを提供します。

#@# Observe the following:
次の例を見てください。

//emlist{
(ns myapp.main
  (:refer-clojure :exclude [min]))

(defn min
  [x y]
  (if (> x y)
    y
    x))
//}

#@# The ns macro also has other directives for loading host classes (with :import) and macros (with :refer-macros), but these are explained in other sections.
@<code>{ns} マクロは、ホストのクラスを読み込むために @<code>{:import} ディレクティブ、マクロを読み込むために @<code>{:refer-macros} ディレクティブを用意しています。これらについては、別のセクションで説明します。


#@# Page56
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


=== 名前空間とファイル名

#@# Namespaces and File Names

#@# When you have a namespace like `myapp.core`, the code must be in a file named _core.cljs_ inside the _myapp_ directory.  So, the preceding examples with namespaces `myapp.core` and `myapp.main` would be found in project with a file structure like this:
@<code>{myapp.core} のような名前空間がある場合、コードは @<code>{myapp} ディレクトリ内で @<code>{core.cljs} という名前のファイルにしなければなりません。上記の例では @<code>{myapp.core} と @<code>{myapp.main} を使用していますが、ファイル構造は次のようになります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
myapp
└── src
    └── myapp
        ├── core.cljs
        └── main.cljs
//}

== 抽象化とポリモーフィズム

#@# Abstractions and Polymorphism

#@# I'm sure that at more than one time you have found yourself in this situation: you have defined a great abstraction (using interfaces or something similar) for your "business logic", and you have found the need to deal with another module over which you have absolutely no control, and you probably were thinking of creating adapters, proxies, and other approaches that imply a great amount of additional complexity.
「インターフェイス等を用いてビジネスロジックの抽象化を上手く定義できたが、コントロールできない別のモジュールを処理する必要がでてきた」

過去にこのような状況を経験したことがあるかと思います。アダプターやプロキシ等の手法で対処することが考えられますが、複雑な作業を追加で行う必要があります。

#@# Some dynamic languages allow "monkey-patching"; languages where the classes are open and any method can be defined and redefined at any time. Also, it is well known that this technique is a very bad practice.
一部の動的言語では「モンキーパッチ」が可能です。そのような言語では、クラスは開かれていて、いつでもメソッドを定義したり再定義したりできます。このテクニックは非常に悪い慣習であることもよく知られています。

#@# We can not trust languages that allow you to silently overwrite methods that you are using when you import third party libraries; you cannot expect consistent behavior when this happens.
サードパーティのライブラリをインポートするときに、使用しているメソッドを暗黙のうちに上書きできる言語は信頼できません。この場合、一貫した動作は期待できません。

#@# These symptoms are commonly called the "expression problem"; see http://en.wikipedia.org/wiki/Expression_problem for more details
これらの問題は Expression Problem として知られています。

詳細は、http://en.wikipedia.org/wiki/Expression_problem を参照してください。


=== プロトコル

#@# Protocols

#@# The ClojureScript primitive for defining "interfaces" is called a protocol. A protocol consists of a name and set of functions. All the functions have at least one argument corresponding to the this in JavaScript or self in Python.
「インターフェイス」を定義するためのClojureScript のプリミティブは、プロトコルと呼ばれています。プロトコルは名前と関数のセットで構成されます。全ての関数には、JavaScript の @<code>{this}、Python の @<code>{self} に対応する引数が少なくとも 1 つはあります。

#@# https://teratail.com/questions/70213

#@# Protocols provide a type-based polymorphism, and the dispatch is always done by the first argument (equivalent to  JavaScript ’s this, as previously mentioned).
#@# A protocol looks like this:
プロトコルは、型に基づくポリモーフィズムを提供して、常に最初の引数によりディスパッチされます（前述の通り、JavaScript の @<code>{this} に相当します）。プロトコルは次のように記述します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(ns myapp.testproto)

(defprotocol IProtocolName
  "プロトコルを記述するdocstring"
  (sample-method [this] "この関数に関連付けられている doc 文字列"))
//}

#@# "A docstring describing the protocol."
#@# "A doc string associated with this function."

#@# Page57
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}  


#@# NOTE: the "I" prefix is commonly used to designate the separation of protocols and types. In the Clojure community, there are many different opinions about how the "I" prefix should be used. In our opinion, it is an acceptable solution to avoid name clashing and possible confusion. But not using the prefix is not considered bad practice.
プロトコルを型と区別するために、接頭辞に I をつける慣習があります。Clojure のコミュニティでは、この接頭辞の使い方について様々な意見があります。私たちの意見では、名前衝突と混乱の可能性を避けるために許容できる解決策です。しかし、この接頭辞を使用しないことは悪しき慣行とはみなされません。

#@# From the user perspective, protocol functions are simply plain functions defined in the namespace where the protocol is defined. This enables an easy and simple aproach for avoid conflicts between different protocols implemented for the same type that have conflicting function names.
ユーザーからすると、プロトコル関数は、プロトコルが定義されている名前空間で定義された単純な関数です。これを用いると、競合する関数名を持つ同じ型のために実装された異なるプロトコル間での競合を回避するために、容易でシンプルなアプローチをとることができます。

#@# Here is an example.  let 's create a protocol called IInvertible for data that can be "inverted".  It will have a single method named invert.
次の例では、@<code>{IInvertible} という名前で、反転できるデータのためのプロトコルを作成します。

//emlist{
(defprotocol IInvertible
  "これは「反転可能な」データ型のためのプロトコルです。"
  (invert [this] "指定した項目を反転します"))
//}

#@# "Invert the given item."

=== 既存の型の拡張

#@# Extending existing types

#@# One of the big strengths of protocols is the ability to extend existing and maybe third party types. This operation can be done in different ways.
#@# The majority of time you will tend to use the *extend-protocol* or the *extend-type* macros. This is how extend-type syntax looks:
プロトコルの大きな強みの 1 つは、既存の型やサードパーティの型を拡張できることです。この操作は、様々な方法で実行できます。
ほとんどの場合、@<code>{extend-type} マクロか @<code>{extend-protocol} マクロを使います。@<code>{extend-type} の構文は次のように記述します。

//emlist{
(extend-type TypeA
  ProtocolA
  (function-from-protocol-a [this]
    ;; ここに実装を記述する
    )

  ProtocolB
  (function-from-protocol-b-1 [this parameter 1]
    ;; ここに実装を記述する
    )
  (function-from-protocol-b-2  [this parameter 1 parameter 2]
    ;; ここに実装を記述する
    ))
//}

//embed[latex]{
\vspace{-0.4\Cvs}
//}
 
#@# You can observe that with *extend-type* you are extending a single type with different protocols in a single expression.
@<code>{extend-type} を使用すると、異なるプロトコルを使用して 1 つの型を 1 つの式で拡張できます。

#@#  let's play with our IInvertible protocol defined previously:
先ほど定義した@<code>{IInvertible}プロトコルを試してみましょう。

//embed[latex]{
\vspace{-0.4\Cvs}
//}

//emlist{
(extend-type string
  IInvertible
  (invert [this] (apply str (reverse this))))
//}


#@# Page58
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{ 
(extend-type cljs.core.List
  IInvertible
  (invert [this] (reverse this)))

(extend-type cljs.core.PersistentVector
  IInvertible
  (invert [this] (into [] (reverse this))))
//}

#@# You may note that a special symbol *string* is used instead of `js/String` for extend the protol for string. This is because the builtin JavaScript types have special treatment and if you replace the string with `js/String` the compiler will emit a warning about that.
プロトコルを文字列に対して拡張するために、@<code>{js/String} を使わずに、特別なシンボル @<code>{string} が使われていることに注目してください。これは、組み込みの JavaScript の型には特別な扱いがあり、@<code>{string} を @<code>{js/String} に置き換えると、コンパイラは警告を発生するからです。

#@# So if you want extend your protocol to JavaScript primitive types, instead of using `js/Number`, `js/String`, `js/Object`, `js/Array`, `js/Boolean` and `js/Function` you should use the respective special symbols: number, string, object, array, boolean and function.
プロトコルを JavaScript のプリミティブ型に拡張する場合、@<code>{js/Number}, @<code>{js/String}, @<code>{js/Object}, @<code>{js/Array}, @<code>{js/Boolean}, @<code>{js/Function} を使う代わりに、それぞれに特別なシンボル(@<code>{number}, @<code>{string}, @<code>{object}, @<code>{array}, @<code>{boolean}, @<code>{function})を使う必要があります。

#@# Now, it's time to try our protocol implementation:
では、私たちのプロトコル実装を試してみましょう。

//emlist{
(invert "abc")
;; => "cba"

(invert 0)
;; => 0

(invert '(1 2 3))
;; => (3 2 1)

(invert [1 2 3])
;; => [3 2 1]
//}

#@# In comparison, *extend-protocol* does the inverse; given a protocol, it adds implementations for multiple types. This is how the syntax looks:
比較すると、@<code>{extend-protocol} はその逆を行います。つまり、あるプロトコルに対して、複数の型の実装を追加します。構文は次のようになります。

//emlist{
(extend-protocol ProtocolA
  TypeA
  (function-from-protocol-a [this]
    ;; ここに実装を記述する
    )

  TypeB
  (function-from-protocol-a [this]
    ;; ここに実装を記述する
    ))
//}


#@# Page59
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 

#@# Thus, the previous example could have been written equally well with this way:
したがって、前例は次のように書くこともできます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(extend-protocol IInvertible
  string
  (invert [this] (apply str (reverse this)))

  cljs.core.List
  (invert [this] (reverse this))

  cljs.core.PersistentVector
  (invert [this] (into [] (reverse this))))
//}


===  ClojureScript での抽象化

#@# Participate in ClojureScript abstractions

#@# ClojureScript itself is built up on abstractions defined as protocols. Almost all behavior in the ClojureScript language itself can be adapted to third party libraries. let 's look at a real life example.
ClojureScript 自体は、プロトコルとして定義された抽象に基づいて構築されています。ClojureScript 言語自体のほぼ全ての動作は、サードパーティ製のライブラリに適用できます。実際の例を見てみましょう。

#@# In previous sections, we have explained the different kinds of built-in collections. For this example we will use a *set*. See this snippet of code:
前のセクションでは、様々な組み込みのコレクションについて説明しました。次の例では、セットを使用します。次のコードを見てください。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def mynums #{1 2})

(filter mynums [1 2 4 5 1 3 4 5])
;; => (1 2 1)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# What happened? In this case, the _set_ type implements the ClojureScript internal IFn protocol that represents an abstraction for functions or anything callable. This way it can be used like a callable predicate in filter.
#@# OK, but what happens if we want to use a regular expression as a predicate function for filtering a collection of strings:
何が起こったのでしょうか。セット型は ClojureScript 内部の @<code>{IFn} プロトコルを実装しています。@<code>{IFn} プロトコルは、関数や呼び出し可能なもののための抽象化を表現します。このように、@<code>{filter} で呼び出し可能な述部のように使用できます。
では、文字列のコレクションを @<code>{filter} する述語関数として正規表現を使用したい場合はどうなるでしょうか。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(filter #"^foo" ["haha" "foobar" "baz" "foobaz"])
;; TypeError: Cannot call undefined
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# The exception is raised because the RegExp type does not implement the IFn protocol so it cannot behave like a callable, but that can be easily fixed:
@<code>{RegExp} の型は @<code>{IFn} プロトコルを実装していないので、例外が発生します。正規表現は呼び出せませんが、次のように簡単に修正することができます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(extend-type js/RegExp
  IFn
  (-invoke
   ([this a]
     (re-find this a))))
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page60
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

 
#@#  let ’s analyze this: we are extending the `js/RegExp` type so that it implements the invoke function in the IFn protocol. To invoke a regular expression a as if it were a function, call the re-find function with the object of the function and the pattern.
この例を分析してみましょう。@<code>{IFn} プロトコルで @<code>{invoke} 関数を実装するように @<code>{js/RegExp} 型を拡張しています。正規表現を関数のように呼び出すには、関数のオブジェクトとパターンを指定して @<code>{re-find} 関数を呼び出します。

#@# Now, you will be able use the regex instances as predicates in a filter operation:
これで、正規表現のインスタンスを @<code>{filter} 操作の述語として使用できるようになります。

//emlist{
(filter #"^foo" ["haha" "foobar" "baz" "foobaz"])
;; => ("foobar" "foobaz")
//}


=== プロトコルを用いた内部分析

#@# Introspection using Protocols

#@# ClojureScript comes with a useful function that allows runtime introspection: satisfies?. The purpose of this function is to determine at runtime if some object (instance of some type) satisfies the concrete protocol.
#@# So, with the previous examples, if we check if a set instance satisfies an *IFn* protocol, it should return true:
ClojureScript には、実行時のイントロスペクション(introspection)を便利にする関数 @<code>{satisfies?} が用意されています。この関数の目的は、実行時に任意のオブジェクト(任意の型のインスタンス)が特定のプロトコルの基準を満たすかを判定することです。
先ほどの例では、セットのインスタンスが @<code>{IFn} プロトコルを満たす場合、@<code>{true} を返します。

//emlist{
(satisfies? IFn #{1})
;; => true
//}

=== マルチメソッド

#@# Multimethods

#@# We have previously talked about protocols which solve a very common use case of polymorphism: dispatch by type. But in some circumstances, the protocol approach can be limiting. And here, *multimethods* come to the rescue.
ポリモーフィズムの非常に一般的なユースケースを解決するプロトコルについて前に説明しました。つまり、型によるディスパッチです。しかし、いくつかの状況では、プロトコルを用いたアプローチには制限があります。そのような場合、マルチメソッドが救いの手を差し伸べてくれます。

#@# These *multimethods* are not limited to type dispatch only; instead, they also offer dispatch by types of multiple arguments and by value. They also allow ad-hoc hierarchies to be defined. Also, like protocols, multimethods are an "Open System", so you or any third parties can extend a multimethod for new types.
マルチメソッドは、型によるディスパッチに限定されません。代わりに、複数の引数の型と値によるディスパッチを提供します。また、特定の目的のために階層性を定義できます。また、プロトコルと同様にマルチメソッドは「開かれたシステム」なので、ユーザやサードパーティは新しい型のためにマルチメソッドを拡張できます。

#@# The basic constructions of *multimethods* are the defmulti and defmethod forms. The defmulti form is used to create the multimethod with an initial dispatch function. This is a model of what it looks like:
マルチメソッドのための基本的な構文は @<code>{defmulti} と @<code>{defmethod} です。@<code>{defmulti} は、イニシャル・ディスパッチ関数(initial dispatch function)を用いてマルチメソッドを作成するために使います。次はそのモデルです。

#@# "A polymorphic function that return a greetings message depending on the language key with default lang as :en"

//emlist{
(defmulti say-hello
  "言語キーに応じて挨拶のメッセージを返すポリモーフィズム的な関数。
   デフォルトの言語キーを en に設定します。"
  (fn [param] (:locale param))
  :default :en)
//}

#@# (defmulti name docstring? attr-map? dispatch-fn & options)


#@# Page61
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

#@# The anonymous function defined within the defmulti form is a dispatch function. It will be called in every call to the say-hello function and should return some kind of marker object that will be used for dispatch. In our example, it returns the contents of the :locale key of the first argument.
@<code>{defmulti} 内で定義されてい無名関数は、ディスパッチ関数です。この関数は @<code>{say-hello} 関数を呼び出すたびに呼び出されて、ディスパッチに使用される何らかのマーカーオブジェクトを返します。この例では、最初の引数の @<code>{:locale} キーの内容を返します。


#@# And finally, you should add implementations. That is done with the defmethod form:
最後に、実装を追加します。@<code>{defmethod} で実装を定義します。

//emlist{
(defmethod say-hello :en
  [person]
  (str "Hello " (:name person "Anonymous")))

(defmethod say-hello :es
  [person]
  (str "Hola " (:name person "Anónimo")))
//}

#@# So, if you execute that function over a hash map containing the :locale and optionally the :name key, the multimethod will first call the dispatch function to determine the dispatch value, then it will search for an implementation for that value. If an implementation is found, the dispatcher will execute it. Otherwise, the dispatch will search for a default implementation (if one is specified) and execute it.
もし @<code>{:locale} と (オプションの)@<code>{:name} キーを含むハッシュマップに対してこの関数を実行すると、マルチメソッドはまずディスパッチ関数を呼び出してディスパッチ値を決定し、次にその値の実装を検索します。実装が見つかった場合、ディスパッチャはそれを実行します。それ以外の場合、ディスパッチはデフォルトの実装(指定されている場合)を検索して実行します。

//emlist{
(say-hello {:locale :es})
;; => "Hola Anónimo"

(say-hello {:locale :en :name "Ciri"})
;; => "Hello Ciri"

(say-hello {:locale :fr})
;; => "Hello Anonymous"
//}

#@# If the default implementation is not specified, an exception will be raised notifying you that some value does not have an implementation for that multimethod.
デフォルトの実装が指定されていない場合は、一部の値がそのマルチメソッドの実装をもたないことを通知する例外が発生します。


=== ヒエラルキー

#@# Hierarchies

#@# Hierarchies are  ClojureScript’s way to let you build whatever relations that your domain may require. Hierarchies are defined in term of relations between named objects, such as symbols, keywords, or types.
ヒエラルキーは、ClojureScript において、ドメインに必要なリレーションを構築することができます。ヒエラルキーは、シンボル、キーワード、型のような名前付きのオブジェクト間のリレーションとして定義されます。

#@# Hierarchies can be defined globally or locally, depending on your needs. Like multimethods, hierarchies are not limited to a single namespace. You can extend a hierarchy from any namespace, not only from the one in which it is defined.
ヒエラルキーは、必要に応じてグローバルまたはローカルに定義できます。マルチメソッドのように、ヒエラルキーは単一の名前空間に限定されません。ヒエラルキーは、定義されている名前空間だけでなく、任意の名前空間から拡張できます。

#@# The global namespace is more limited, for good reasons. Keywords or symbols that are not namespaced can not be used in the global hierarchy. That behavior helps prevent unexpected situations when two or more third party libraries use the same symbol for different semantics.
グローバルの名前空間はより制限されていますが、これは正当な理由によるものです。名前空間を持たないキーワードまたはシンボルは、グローバルのヒエラルキーでは使用できません。この動作は、複数のサードパーティ製のライブラリが異なるセマンティクスに対して同じシンボルを使用する場合、予期しない状況が発生するのを防ぐのに役立ちます。

#@# Page62
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


===== ヒエラルキーの定義

#@# Defining a hierarchy

#@# The hierarchy relations should be established using the derive function:
ヒエラルキーのリレーションは、@<code>{derive} 関数を用いて確立する必要があります。

//emlist{
(derive ::circle ::shape)
(derive ::box ::shape)
//}

#@# We have just defined a set of relationships between namespaced keywords. In this case the ::circle is a child of ::shape, and ::box is also a child of ::shape.
ここでは、名前空間を持つキーワード間のリレーションを定義しました。この場合、@<code>{::circle} は @<code>{::shape} の子であり、@<code>{::box} も @<code>{::shape} の子です。

#@# TIP: The ::circle keyword syntax is a shorthand for `:current.ns/circle`.
#@# So if you are executing it in a REPL, ::circle will be evaluated as `:cljs.user/circle`.
@<code>{::circle} は @<code>{:current.ns/circle} の省略形なので、
REPLで実行する場合、@<code>{::circle} は @<code>{:current.ns/circle} として評価されます。

===== ヒエラルキーとイントロスペクション

#@# Hierarchies and introspection

#@# ClojureScript comes with a little toolset of functions that allows runtime introspection of globally or locally defined hierarchies. This toolset consists of three functions: isa?, ancestors, and descendants.
#@# let 's see an example of how it can be used with the hierarchy defined in the previous example:
ClojureScript には、グローバルあるいはローカルに定義されたヒエラルキーのイントロスペクションを実行時に可能にする小さなツールセットが用意されています。このツールセットは 3 つの関数で構成されています。@<code>{isa?}, @<code>{ancestors}, @<code>{descendants} です。
前例で定義したヒエラルキーを用いて、このツールセットの使い方を見てみましょう。

//emlist{
(ancestors ::box)
;; => #{:cljs.user/shape}

(descendants ::shape)
;; => #{:cljs.user/circle :cljs.user/box}

(isa? ::box ::shape)
;; => true

(isa? ::rect ::shape)
;; => false
//}


===== ローカル環境に定義されたヒエラルキー

#@# Locally defined hierarchies

#@# As we mentioned previously, in ClojureScript you also can define local hierarchies. This can be done with the make-hierarchy function. Here is an example of how you can replicate the previous example using a local hierarchy:
前に説明した通り、ClojureScript はローカルのヒエラルキーを定義することもできます。これは、@<code>{make-hierarchy} 関数により行うことができます。次に、前例をローカルのヒエラルキーを用いて書き直してみましょう。

//embed[latex]{
\vspace{-0.5\Cvs}
//}

//emlist{
(def h (-> (make-hierarchy)
           (derive :box :shape)
           (derive :circle :shape)))
//}

//embed[latex]{
\vspace{-0.5\Cvs}
//} 
 
#@# Now you can use the same introspection functions with that locally defined hierarchy:
これで、ローカルに定義されたヒエラルキーで同じ関数を使用できます。

//embed[latex]{
\vspace{-0.5\Cvs}
//} 

//emlist{
(isa? h :box :shape)
;; => true
//}

 
#@# Page63
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(isa? :box :shape)
;; => false
//}

#@# As you can observe, in local hierarchies we can use normal (not namespace qualified) keywords, and if we execute the isa? without passing the local hierarchy parameter, it returns false as expected.
ご覧のように、ローカルのヒエラルキーでは、通常の(名前空間が修飾されない)キーワードを使用できます。もし @<code>{isa?} をローカルのヒエラルキーの引数を渡さずに実行すると、予想した通りに @<code>{false} を返します。

===== マルチスレッドにおけるヒエラルキー

#@# Hierarchies in multimethods

#@# One of the big advantages of hierarchies is that they work very well together with multimethods.  This is because multimethods by default use the isa? function for the last step of dispatching.
ヒエラルキーの大きな利点の 1 つは、マルチメソッドと上手く働くことです。これは、マルチメソッドがデフォルトで @<code>{isa?} 関数をディスパッチの最終ステップで使うからです。

#@# let 's see an example to clearly understand what that means. First, we define the multimethod with the defmulti form:
その意味を明確に理解するために例を見てみましょう。初めに、@<code>{defmulti} を用いてマルチメソッドを定義します。

//emlist{
(defmulti stringify-shape
  "shape キーワードを人が読めるような表現で出力する関数"
  identity
  :hierarchy #'h)
//}

#@# "A function that prints a human readable representation of a shape keyword."

#@# With the :hierarchy keyword parameter, we indicate to the multimethod what hierarchy we want to use; if it is not specified, the global hierarchy will be used.
@<code>{:hierarchy} キーワードパラメータを使用して、使用するヒエラルキーをマルチメソッドに指定します。指定しない場合は、グローバルのヒエラルキーが使用されます。

#@# Second, we define an implementation for our multimethod using the defmethod form:
次に、@<code>{defmethod} を用いて、マルチメソッドのための実装を定義します。

//emlist{
(defmethod stringify-shape :box
  [_]
  "A box shape")

(defmethod stringify-shape :shape
  [_]
  "A generic shape")

(defmethod stringify-shape :default
  [_]
  "Unexpected object")
//}

#@# Now, let's see what happens if we execute that function with a box:
では、関数を @<code>{:box} で呼び出したときに何が起こるかを見てみましょう。

//emlist{
(stringify-shape :box)
;; => "A box shape"
//}


#@# Page64
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


#@# Now everything works as expected; the multimethod executes the direct matching implementation for the given parameter. Next,  let 's see what happens if we execute the same function but with the :circle keyword as the parameter which does not have the direct matching dispatch value:
全て予想通りに、マルチメソッドは与えられた引数に適する実装を直接実行しました。次に @<code>{:circle} を用いて同じ関数を呼び出してみましょう。この場合、@<code>{:circle} に合致する実装はありません。

//emlist{
(stringify-shape :circle)
;; => "A generic shape"
//}

#@# The multimethod automatically resolves it using the provided hierarchy, and since :circle is a descendant of :shape, the :shape implementation is executed.
マルチメソッドは指定されたヒエラルキーを用いて自動的に解決されます。@<code>{:circle} は @<code>{:shape} の子孫(descendant)であるため、@<code>{:shape} の実装が呼び出されます。

#@# Finally, if you give a keyword that isn't part of the hierarchy, you get the :default implementation:
ヒエラルキーの一部ではないキーワードを指定すると、@<code>{:default} の実装が呼び出されます。


//emlist{
(stringify-shape :triangle)
;; => "Unexpected object"
//}

== データ型

#@# Data types

#@# Until now, we have used maps, sets, lists, and vectors to represent our data. And in most cases, this is a really great approach. But sometimes we need to define our own types, and in this book we will call them *data types*.
ここまでは、マップ、セット、リスト、ベクタをデータを表現するために使ってきました。多くの場合、これは本当に素晴らしいアプローチです。しかし、自分自身でこのような型を定義する必要があるときもあります。この本では、このような型のことを「データ型」と呼びます。

これまでは、マップ、セット、リスト、ベクタを使ってデータを表現してきました。ほとんどの場合、これは本当に素晴らしいアプローチです。しかし、場合によっては、独自に型を定義する必要があります。本書ではそれらをデータ型と呼ぶことにします。

#@# A data type provides the following:
データ型は、次の機能を提供します。

#@# * A unique host-backed type, either named or anonymous.
#@# * The ability to implement protocols (inline).
#@# * Explicitly declared structure using fields or closures.
#@# * Map-like behavior (via records, see below).

- ホスト環境が支援する独自の型(名前付きまたは無名)

- プロトコルを実装する機能(インライン)

- フィールドまたはクロージャを用いた明示的に宣言された構造

- マップのような動作(レコードを経由。詳細は後ほど。)


=== Deftype

#@# The most low-level construction in ClojureScript for creating your own types is the deftype macro. As a demonstration, we will define a type called User:
ClojureScript で独自の型を作成するための最も低レベルな構文は @<code>{deftype} マクロです。例として、@<code>{User} という型を定義してみましょう。

//emlist{
(deftype User [firstname lastname])
//}


#@# Once the type has been defined, we can create an instance of our User. In the following example, the `.` after User indicates that we are calling a constructor.
型を定義すると、@<code>{User} のインスタンスを作成できます。次の例では、@<code>{User} の後のドット @<code>{.} はコンストラクタを呼び出していることを示します。


#@# Page65
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{
(def person (User. "Triss" "Merigold"))
//}

#@# Its fields can be accessed using the prefix dot notation:
フィールドには @<code>{.-} でアクセスできます。

//emlist{
(.-firstname person)
;; => "Triss"
//}

#@# Types defined with deftype (and defrecord, which we will see later) create a host-backed class-like object associated with the current namespace. For convenience, ClojureScript also defines a constructor function called `->User` that can be imported using the :require directive.
@<code>{deftype}で定義された型は、ホスト環境に支援されるクラスのようなオブジェクトを現在の名前空間と関連づけて作成します。これは @<code>{defrecord} で定義される型も同様です。@<code>{defrecord} については後に取り上げます。利便性のために ClojureScript は、@<code>{:require} ディレクティブでインポートできる @<code>{->User} というコンストラクタ関数も定義しています。

#@# We personally do not like this type of function, and we prefer to define our own constructors with more idiomatic names:
私たちは個人的にはこのタイプの関数は好きではないため、より慣用的な名前で独自のコンストラクターを定義したいと思います。

//emlist{
(defn make-user
  [firstname lastname]
  (User. firstname lastname))
//}

#@# We use this in our code instead of `->User`.
私たちのコードでは @<code>{->User} の代わりにこちらを使います。

=== Defrecord

#@# The record is a slightly higher-level abstraction for defining types in ClojureScript and should be the preferred way to do it.
レコード(record)は、ClojureScript で型を定義するための少し高いレベルの抽象化であり、型の定義のために推奨されるべき方法です。

#@# As we know, ClojureScript tends to use plain data types such as maps, but in most cases we need a named type to represent the entities of our application. Here come the records.
私たちが知っているように、ClojureScript ではマップなどのプレーンなデータ型を使用する傾向がありますが、ほとんどの場合、アプリケーションの実体を表す名前付きの型が必要です。そこでレコードの出番です。

#@# A record is a data type that implements the map protocol and therefore can be used like any other map.  And since records are also proper types, they support type-based polymorphism through protocols.
#@# In summary: with records, we have the best of both worlds, maps that can play in different abstractions.
レコードはマッププロトコルを実装するデータ型であるため、マップと同様に使用できます。また、レコードも適切な型であるため、プロトコルを介して型ベースのポリモーフィズムをサポートします。
要約すると、レコードを用いると、様々な抽象化で働くマップという、両方の世界の良い点をもつことができます。

#@#  let's start defining the User type but using records:
レコードを使用して @<code>{User} 型の定義を始めましょう。

//emlist{
(defrecord User [firstname lastname])
//}


#@# Page66
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 

#@# It looks really similar to the deftype syntax; in fact, it uses deftype behind the scenes as a low-level primitive for defining types.
@<code>{deftype} の構文とよく似ています。実際には、型を定義するための下位レベルのプリミティブとして、舞台裏では @<code>{deftype} を使用しています。

#@# Now, look at the difference with raw types for access to its fields:
ここで、フィールドへのアクセスについて、素の型の場合との違いを見てみましょう。


//emlist{
(def person (User. "Yennefer" "of Vengerberg"))

(:firstname person)
;; => "Yennefer"

(get person :firstname)
;; => "Yennefer"
//}

 
#@# As we mentioned previously, records are maps and act like them:
レコードはマップであり、マップのように扱うことができます。
前述の通り、レコードはマップであり、マップのように動作します。

//emlist{
(map? person)
;; => true
//}

#@# And like maps, they support extra fields that are not initially defined:
また、マップと同様に、レコードは最初に定義されていないフィールドを追加できます。

//emlist{
(def person2 (assoc person :age 92))

(:age person2)
;; => 92 
//}

#@# As we can see, the assoc function works as expected and returns a new instance of the same type but with new key value pair. But take care with dissoc! Its behavior with records is slightly different than with maps; it will return a new record if the field being dissociated is an optional field, but it will return a plain map if you dissociate a mandatory field.
#@# Another difference with maps is that records do not act like functions:
このように、@<code>{assoc} 関数は期待どおりに動作し、同じ型の新しいインスタンスを、新しいキーと値のペアで返します。ただし、@<code>{dissoc} には注意してください。レコードでの動作は、マップでの動作とは少し異なります。外すフィールドがオプションのフィールドである場合は新しいレコードを返しますが、必須のフィールドを外す場合はマップを返します。
マップとのもう 1 つの違いは、レコードが関数のように動作しないことです。

//emlist{
(def plain-person {:firstname "Yennefer", :lastname "of Vengerberg"})

(plain-person :firstname)　;; => "Yennefer"

(person :firstname)　　　　 ;; => person.User does not implement IFn protocol.
//}
 

#@# Page67
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


#@# For convenience, the defrecord macro, like deftype, exposes a `->User` function, as well as an additional `map->User` constructor function. We have the same opinion about that constructor as with deftype defined ones: we recommend defining your own instead of using the other ones. But as they exist,  let ’s see how they can be used:
利便性のために、@<code>{deftype} と同様に、@<code>{defrecord} マクロは @<code>{->User} 関数と追加の @<code>{map->User} コンストラクタ関数を公開しています。このコンストラクタについては、@<code>{deftype} で定義したコンストラクタと同じ意見です。他のコンストラクタを使用する代わりに、独自のコンストラクタを定義することをお勧めします。それらが存在する場合、どのように使用できるかを見てみましょう。

//emlist{
(def cirilla (->User "Cirilla" "Fiona"))
(def yen (map->User {:firstname "Yennefer"
                     :lastname "of Vengerberg"}))
//}

=== プロトコルの実装

#@# Implementing protocols

#@# Both type definition primitives that we have seen so far allow inline implementations for protocols (explained in a previous section).  let's define one for example purposes:
これまで見てきた両方の型定義のプリミティブは、(前に説明した通り)プロトコルのインライン実装を可能にします。例として、次のように定義します。

//emlist{
(defprotocol IUser
  "User 型を操作するための共通の抽象化"
  (full-name [_] "Get the full name of the user."))
//}

#@# "A common abstraction for working with user types."

#@# Now, you can define a type with inline implementation for an abstraction, in our case the IUser:
ここでは、抽象化のために、インライン実装 (この場合は@<code>{IUser}) を使用して型を定義できます。

//emlist{
(defrecord User [firstname lastname]
  IUser
  (full-name [_]
    (str firstname " " lastname)))

;; Create an instance.
(def user (User. "Yennefer" "of Vengerberg"))

(full-name user)
;; => "Yennefer of Vengerberg"
//}


=== Reify

#@# reify macro is an _ad hoc constructor_ you can use to create objects without pre-defining a type.  Protocol implementations are supplied the same as deftype and defrecord, but in contrast, reify does not have accessible fields.
@<code>{reify} マクロは、型を事前に定義せずにオブジェクトを作成するために使用できる特別なコンストラクタです。プロトコルの実装は @<code>{deftype} や @<code>{defrecord} と同じように提供されますが、対照的に @<code>{reify} はアクセス可能なフィールドをもちません。


#@# This is how we can emulate an instance of the user type that plays well with the IUser abstraction:
@<code>{IUser} の抽象化でうまく機能するユーザ型のインスタンスをエミュレートするには、次のようにします。


#@# Page68
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{
(defn user
  [firstname lastname]
  (reify
    IUser
    (full-name [_]
      (str firstname " " lastname))))

(def yen (user "Yennefer" "of Vengerberg"))
(full-name yen)
;; => "Yennefer of Vengerberg"
//}


=== Specify

#@# specify! is an advanced alternative to reify, allowing you to add protocol implementations to an existing JavaScript object.  This can be useful if you want to graft protocols onto a JavaScript library's components.
@<code>{specify!} は @<code>{reify} の高度な代替手段であり、既存の JavaScript オブジェクトにプロトコルの実装を追加できます。これは、JavaScript のライブラリのコンポーネントにプロトコルを移植する場合に便利です。

//emlist{
(def obj #js {})

(specify! obj
  IUser
  (full-name [_]
    "my full name"))

(full-name obj)
;; => "my full name"
//}

#@# specify is an immutable version of specify! that can be used on immutable, copyable values implementing ICloneable (e.g. ClojureScript collections).
@<code>{specify} は @<code>{specify!} の不変版です。これは @<code>{ICloneable} を実装している不変でコピー可能な値(ClojureScript のコレクション等)に対して使えます。

//emlist{
(def a {})

(def b (specify a
         IUser
         (full-name [_]
           "my full name")))

(full-name a)
;; Error: No protocol method IUser.full-name
;; defined for type cljs.core/PersistentArrayMap: {}

(full-name b)
;; => "my full name"
//}


#@# Page69
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 


== ホスト環境の相互運用性

#@# Host interoperability

#@# ClojureScript, in the same way as its brother Clojure, is designed to be a "guest" language. This means that the design of the language works well on top of an existing ecosystem such as JavaScript for ClojureScript and the JVM for _Clojure_.
ClojureScript は、その兄弟である Clojure と同様に、ゲスト言語として設計されています。これは、ClojureScript にとっては JavaScript、Clojure にとっては JVM といった既存のエコシステムの上で、言語の設計がうまく機能することを意味します。

=== 型

#@# The types

#@# ClojureScript , unlike what you might expect, tries to take advantage of every type that the platform provides. This is a (perhaps incomp let e) list of things that ClojureScript inherits and reuses from the underlying platform:
ClojureScript は、予想と異なるかもしれませんが、プラットホームが提供する全ての型を利用しようとします。次は、ClojureScript の基礎となるプラットフォームから継承して再利用する機能の一覧です。

@<embed>{|latex|\vspace{1\Cvs\}}

//noindent
#@# * ClojureScript strings are JavaScript *Strings*.
- ClojureScript の string は、JavaScript の String である。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript numbers are JavaScript *Numbers*.
- ClojureScript の number は、JavaScript の Number である。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript nil is a JavaScript *null*.
- ClojureScript の @<code>{nil} は、JavaScript の null である。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript regular expressions are JavaScript RegExp instances.
- ClojureScript の正規表現は、JavaScript の @<code>{RegExp} のインスタンスである。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript is not interpreted; it is always compiled down to JavaScript.
- ClojureScript はインタープリタで実行されず、常に JavaScript にコンパイルされる。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript allows easy call to platform APIs with the same semantics.
- ClojureScript はプラットフォームの API を同じセマンティクスで簡単に呼び出せる。

@<embed>{|latex|\vspace{0.5\Cvs\}}

//noindent
#@# * ClojureScript data types internally compile to objects in  JavaScript.
- ClojureScript のデータ型は、内部的には JavaScript のオブジェクトにコンパイルされる。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# On top of it, ClojureScript builds its own abstractions and types that do not exist in the platform, such as Vectors, Maps, Sets, and others that are explained in preceding sections of this chapter.
ClojureScript はその上に、ベクタ、マップ、セットなど、本章のこれまでのセクションで説明したプラットフォームに存在しない独自の抽象化や型を構築します。

=== プラットホームの型との連携

#@# Interacting with platform types

#@# ClojureScript comes with a little set of special forms that allows it to interact with platform types such as calling object methods, creating new instances, and accessing object properties.
ClojureScript には、オブジェクトメソッドの呼び出し、新しいインスタンスの作成、オブジェクトプロパティへのアクセスなど、プラットフォームの型との対話を可能にするスペシャルフォームの小さなセットが付属しています。

=== プラットホームへのアクセス

#@# Access to the platform

#@# ClojureScript has a special syntax for access to the entire platform environment through the `js/` special namespace. This is an example of an expression to execute  JavaScript 's built-in parseInt function:
ClojureScript には、特殊な名前空間 @<code>{js/} を通してプラットフォームの環境全体にアクセスできます。JavaScript の組み込み関数 @<code>{parseInt} を実行する式は次の通りです。

//emlist{
(js/parseInt "222")
;; => 222
//}


#@# Page70
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 
 

=== 新たにインスタンスを生成する

#@# Creating new instances

#@# ClojureScript has two ways to create instances:
ClojureScriptにはインスタンスを作成する方法が 2 通りあります。
まずは、@<code>{new} を使う方法です。

//emlist{
(new js/RegExp "^foo$")
//}

#@# Using the `.` special form
ドット @<code>{.} を使うこともできます。

//emlist{
(js/RegExp. "^foo$")
//}

#@# The last one is the recommended way to create instances. We are not aware of any real differences between the two forms, but in the ClojureScript community, the last one is used most often.
後者の方法でインスタンスを作成することをお勧めします。2 つの形式の間に違いはありませんが、ClojureScript コミュニティでは後者の形式が最も頻繁に使用されています。

=== インスタンスのメソッド呼び出し

#@# Invoke instance methods

#@# To invoke methods of some object instance, as opposed to how it is done in JavaScript (e.g., `obj.method()`, the method name comes first like any other standard function in Lisp languages but with a little  var iation: the function name starts with special form `.`.
インスタンスメソッドを呼び出すには、JavaScript の方法(@<code>{obj.method()}の形式)とは異なり、Lisp 系言語の他の標準関数と同じように最初に来ますが、関数名は特殊な形式 @<code>{.} で始まります。

#@# let's see how we can call the `.test()` method of a regexp instance:
正規表現のインスタンスの @<code>{.test()}メソッドを呼び出すには次のようにします。

//emlist{
(def re (js/RegExp "^Clojure"))

(.test re "ClojureScript")
;; => true
//}

#@# You can invoke instance methods on JavaScript objects. The first example follows the pattern you have seen; the last one is a shortcut:
JavaScript のオブジェクトに対してインスタンスメソッドを呼び出すことができます。以下の例において、前者はこれまで見てきたパターンに従っています。後者は前者のショートカットです。

//emlist{
(.sqrt js/Math 2)
;; => 1.4142135623730951
(js/Math.sqrt 2)
;; => 1.4142135623730951
//}


#@# Page71
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 


=== オブジェクトのプロパティへのアクセス

#@# Access to object properties

#@# Access to an object's properties is really very similar to calling a method. The difference is that instead of using the `.` you use `.-`.  let 's see an example:
オブジェクトのプロパティにアクセスする方法は、メソッドを呼び出す方法とほぼ同じです。違いは、ドット @<code>{.} の代わりに ドットハイフン @<code>{.-} を使うことです。例を見てみましょう。

//emlist{
(.-multiline re)
;; => false
(.-PI js/Math)
;; => 3.141592653589793
//}


=== プロパティへアクセスするための略記法

#@# Property access shorthand

#@# Symbols with the `js/` prefix can contain dots to denote nested property access. Both of the following expressions invoke the same function:
@<code>{js/} で始まるシンボルは、ネストされたプロパティのアクセスを表示するためにドットを含むことができます。次の例では、両者とも同じ関数を呼び出します。

//emlist{
(.log js/console "Hello World")

(js/console.log "Hello World")
//}

#@# And both of the following expressions access the same property:
次の式は同じプロパティにアクセスします。

//emlist{
(.-PI js/Math)
;; => 3.141592653589793

js/Math.PI
;; => 3.141592653589793
//}


===  JavaScript のオブジェクト

#@# JavaScript objects

#@# ClojureScript has different ways to create plain JavaScript objects; each one has its own purpose. The basic one is the js-obj function. It accepts a  variable number of pairs of keys and values and returns a JavaScript object:
ClojureScript には、プレーンな JavaScript のオブジェクトを作成する様々な方法があり、各々に目的があります。基本的なものは @<code>{js-obj} 関数です。このメソッドは、可変数のキーと値のペアを受け入れて、JavaScript のオブジェクトを返します。

//emlist{
(js-obj "country" "FR")
;; => #js {:country "FR"}
//}


#@# Page72
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


#@# The return value can be passed to some kind of third party library that accepts a plain JavaScript object, but you can observe the real representation of the return value of this function. It is really another form for doing the same thing.
返り値は、プレーンな JavaScript のオブジェクトを受け入れる何らかのサードパーティのライブラリに渡すことができますが、この関数の戻り値の実際の表現を見ることができます。まったく同じことをするための別の形式です。

#@# Using the reader macro `#js` consists of prepending it to a ClojureScript map or vector, and the result will be transformed to plain  JavaScript :
リーダーマクロの @<code>{#js} を使うことも可能です。マップかベクタの先頭に @<code>{#js} をつけることで、結果が JavaScript のオブジェクトに変換されます。

//emlist{
(def myobj #js {:country "FR"})
//}

#@# The translation of that to plain JavaScript is similar to this:
上の例で変換されるものは、次のコードのようになります。

//emlist{
var myobj = {country: "FR"};
//}

#@# As explained in the previous section, you can also access the plain object properties using the `.-` syntax:
前のセクションで説明した通り、オブジェクトのプロパティにはドットハイフン @<code>{.-} を用いてアクセスできます。

//emlist{
(.-country myobj)
;; => "FR"
//}

#@# And as JavaScript objects are mutable, you can set a new value for some property using the set! function:
JavaScript のオブジェクトはミュータブルなので、@<code>{set!} 関数で新たな値を設定できます。

//emlist{
(set! (.-country myobj) "KR")
//}


=== 変換

#@# Conversions

#@# The inconvenience of the previously explained forms is that they do not make recursive transformations, so if you have nested objects, the nested objects will not be converted.  Consider this example that uses ClojureScript maps, then a similar one with JavaScript objects:
前述のフォームの不便な点は、再帰的な変換を行わないため、ネストされたオブジェクトがある場合、ネストされたオブジェクトは変換されないことです。ClojureScript のマップを使う場合と、JavaScript のマップを使う場合を比べてみましょう。

//emlist{
(def clj-map {:country {:code "FR" :name "France"}})
;; => {:country {:code "FR", :name "France"}}
(:code (:country clj-map))
;; => "FR"
//}


#@# Page73
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


//emlist{
(def js-obj #js {:country {:code "FR" :name "France"}})
;; => #js {:country {:code "FR", :name "France"}
(.-country js-obj)
;; => {:code "FR", :name "France"}
(.-code (.-country js-obj))
;; => nil
//}

#@# To solve that use case, ClojureScript comes with the `clj->js` and `js->clj` functions that transform Clojure collection types into JavaScript and back. Note that the conversion to ClojureScript changes the :country keyword to a string.
このユースケースを解決するために、@<code>{clj->js} と @<code>{js->clj} が用意されています。それぞれ、ClojureScript と JavaScript のコレクションを相互に変換します。ClojureScript への変換で @<code>{:country} キーワードが文字列に変換されている点に注意してください。

//emlist{
(clj->js {:foo {:bar "baz"}})
;; => #js {:foo #js {:bar "baz"}}
(js->clj #js {:country {:code "FR" :name "France"}}))
;; => {"country" {:code "FR", :name "France"}}
//}

#@# In the case of arrays, there is a specialized function into-array that behaves as expected:
配列の場合、期待通りに動作する @<code>{into-array} という特別な関数があります。

//emlist{
(into-array ["France" "Korea" "Peru"])
;; => #js ["France" "Korea" "Peru"]
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

=== 配列

#@# Arrays

#@# In the previous example, we saw how we can create an array from an existing ClojureScript collection. But there is another function for creating arrays: make-array.
#@# .Creating a preallocated array with length 10
前例では、既存の ClojureScript コレクションから配列を作成しましたが、別の方法として @<code>{make-array} 関数を使う方法もあります。次の例では、長さが 10 の事前に割り当てられた配列を作成します。

//emlist{
(def a (make-array 10))
;; => #js [nil nil nil nil nil nil nil nil nil nil]
//}

#@# In ClojureScript, arrays also play well with sequence abstractions, so you can iterate over them or simply get the number of elements with the count function:
ClojureScriptでは、配列はシーケンスの抽象化にも適しているため、配列を繰り返し処理したり、@<code>{count} 関数で単純に要素の数を取得したりすることができます。

//emlist{
(count a)
;; => 10
//}


#@# Page74
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//} 


#@# As arrays in the JavaScript platform are a mutable collection type, you can access a concrete index and set the value at that position:
JavaScript のプラットフォームでは、配列はミュータブルなコレクション型であるため、特定のインデックスにアクセスし、その位置に値を設定できます。

//emlist{
(aset a 0 2)
;; => 2 
a
;; => #js [2 nil nil nil nil nil nil nil nil nil]
//}

#@# Or access in an indexed way to get its values:
もしくは、値を得るために、インデックスを指定してアクセスできます。

//emlist{
(aget a 0)
;; => 2 
//}

#@# In  JavaScript , array index access is equivalent to object property access, so you can use the same functions for interacting with plain objects:
JavaScript では、配列においてインデックスでアクセスすることは、オブジェクトのプロパティへのアクセスと同じであるため、プレーンなオブジェクトと連携するために同じ関数を使うことができます。

//emlist{
(def b #js {:hour 16})
;; => #js {:hour 16}

(aget b "hour")
;; => 16

(aset b "minute" 22)
;; => 22

b
;; => #js {:hour 16, :minute 22}
//}


== 状態管理

#@# State management

#@# We've learned that one of  ClojureScript 's fundamental ideas is immutability. Both scalar values and collections are immutable in  ClojureScript , except those mutable types present in the JS host like Date.
ClojureScript の根本的なアイデアの 1 つが不変性にあることを学びました。ClojureScript ではスカラー値もコレクションも不変ですが、@<code>{Date} のように JS のホスト環境に存在する可変型は例外です。

#@# Immutability has many great properties but we are sometimes faced with the need to model values that change over time. How can we achieve this if we can't change data structures in place?
不変性は多くの優れた特性がありますが、時とともに変化する値をモデル化する必要に迫られることもあります。データ構造を適切に変更できない場合、これを実現するにはどうすればよいでしょうか。


#@# Page75
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 

===  var

#@# var s can be redefined at will inside a namespace but there is no way to know *when* they change. The inability to redefine  var s from other namespaces is a bit limiting; also, if we are modifying state, we're probably interested in knowing when it occurs.
var は名前空間内で自由に再定義できますが、いつ変更されるかを知る方法はありません。var を他の名前空間から再定義できないことは、やや制限的です。また、状態を変更している場合は、それがいつ発生するかを知りたいと思うでしょう。


=== アトム

#@# Atoms

#@# ClojureScript gives us the Atom type, which is an object containing a value that can be altered at will. Besides altering its value, it also supports observation through watcher functions that can be attached and detached from it and validation for ensuring that the value contained in the atom is always valid.
自由に変更できる値を含むオブジェクトとして、ClojureScript は @<code>{Atom} 型を提供します。値を変更するだけでなく、付加したり切り離したりできる watcher 関数による監視や、アトムに含まれる値が常に有効であることを確認するバリデーションもサポートします。

#@# If we were to model an identity corresponding to a person called Ciri, we could wrap an immutable value containing Ciri's data in an atom. Note that we can get the atom's value with the deref function or using its shorthand @ notation:
もし Ciri という名前の人に対応するアイデンティティをモデル化するとすれば、Ciriのデータを含む不変な値を 1 つのアトムでラップすることができます。アトムの値は、@<code>{deref} 関数またはその短縮表記 @<code>{@} を使用して取得できます。

//emlist{
(def ciri (atom {:name "Cirilla" :lastname "Fiona" :age 20}))
;; #<Atom: {:name "Cirilla", :lastname "Fiona", :age 20}>

(deref ciri)
;; {:name "Cirilla", :lastname "Fiona", :age 20}

@ciri
;; {:name "Cirilla", :lastname "Fiona", :age 20}
//}

#@# We can use the swap! function on an atom to alter its value with a function.
#@# Since Ciri's birthday is today, let 's increment her age count:
@<code>{swap!} 関数を値を変更するためにアトムに使うことができます。Ciri の誕生日が今日なので、彼女の年齢 age に加算しましょう。

//emlist{
(swap! ciri update :age inc)
;; {:name "Cirilla", :lastname "Fiona", :age 21}

@ciri
;; {:name "Cirilla", :lastname "Fiona", :age 21}
//}

#@# The reset! functions replaces the value contained in the atom with a new one:
@<code>{reset!} 関数は、アトムに含まれる値を新しい値で置き換えます。

//emlist{
(reset! ciri {:name "Cirilla", :lastname "Fiona", :age 22})
;; {:name "Cirilla", :lastname "Fiona", :age 22}

@ciri
;; {:name "Cirilla", :lastname "Fiona", :age 22}
//}


#@# Page76
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//} 

@<embed>{|latex|\vspace{-0.4\Cvs\}}

=== 監視

#@# Observation

#@# https://ejje.weblio.jp/content/External+observation

#@# We can add and remove watcher functions for atoms. Whenever the atom's value is changed through a swap! or reset!, all the atom's watcher functions will be called. Watchers are added with the add-watch function. Notice that each watcher has a key associated (:logger in the example) to it which is later used to remove the watch from the atom.
アトムの監視関数を追加したり削除したりできます。アトムの値が @<code>{swap!} や @<code>{reset!} によって変更されるたびに、アトムの監視関数を呼び出されます。監視は、@<code>{add-watch} 関数を用いて追加できます。それぞれの watcher には、後でアトムから監視を削除するために使用されるキー(例では @<code>{:logger})が関連付けられていることに注意してください。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def a (atom))
(add-watch a :logger (fn [key the-atom old-value new-value]
                       (println "Key:" key "Old:" old-value "New:" new-value)))

(reset! a 42)
;; Key: :logger Old: nil New: 42 
;; => 42 

(swap! a inc)
;; Key: :logger Old: 42  New: 43
;; => 43

(remove-watch a :logger)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

=== Volatile

#@# Volatiles

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Volatiles, like atoms, are objects containing a value that can be altered. However, they don't provide the observation and validation capabilities that atoms provide. This makes them slightly more performant and a more suitable mutable container to use inside stateful functions that don't need observation nor validation.
Volatile は、アトムと同様に、変更可能な値を含むオブジェクトです。しかし、Volatile はアトムが提供するような監視やバリデーションの機能をもっていません。これにより、多少パフォーマンスが向上するため、監視やバリデーションが不要なミュータブルなコンテナに向いています。

#@# Their API closely resembles that of atoms. They can be dereferenced to grab the value they contain and support swapping and resetting with vswap! and vreset! respectively:
Volatile の API はアトムのものとよく似ています。それらは、含まれる値を取得するために、参照を解除して値を取得できます。@<code>{vswap!} でスワップを、@<code>{vreset!} でリセットをサポートします。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def ciri (volatile! {:name "Cirilla" :lastname "Fiona" :age 20}))
;; #<Volatile: {:name "Cirilla", :lastname "Fiona", :age 20}>
(volatile? ciri)
;; => true
(deref ciri)
;; {:name "Cirilla", :lastname "Fiona", :age 20}

(vswap! ciri update :age inc)
;; {:name "Cirilla", :lastname "Fiona", :age 21}
(vreset! ciri {:name "Cirilla", :lastname "Fiona", :age 22})
;; {:name "Cirilla", :lastname "Fiona", :age 22}
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Note that another difference with atoms is that the constructor of volatiles uses a bang at the end. You create volatiles with volatile! and atoms with atom.
アトムとのもう 1 つの違いは、Volatiles のコンストラクタは、最後に @<code>{!} をつける点です。Volatile を作成するには @<code>{volatile!} を、アトムを作るには @<code>{atom} を使います。

//embed[latex]{
\enlargethispage{20mm}
//}
