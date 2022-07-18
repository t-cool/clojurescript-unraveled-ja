= ClojureScript 発展編

#@# Language Advanced
#@# Page103

== Transducer

=== データの変換

#@# Data transformation

#@# ClojureScript offers a rich vocabulary for data transformation in terms of the sequence abstraction, which makes such transformations highly general and composable. Let's see how we can combine several collection processing functions to build new ones. We will be using a simple problem throughout this section: splitting grape clusters, filtering out the rotten ones, and cleaning them. We have a collection of grape clusters like the following:
ClojureScript では、シーケンス抽象化の観点から、データ変換のための豊富な語彙が用意されています。コレクション処理の関数を組み合わせて新しい関数を作成する方法を見てみましょう。この章では、ブドウの房(cluster)を分け、腐った(rotten)房を分別して、きれいにするという単純な問題を扱います。次のようなブドウの房のコレクションがあります。

//emlist{
(def grape-clusters
  [{:grapes [{:rotten? false :clean? false}
             {:rotten? true :clean? false}]
    :color :green}
   {:grapes [{:rotten? true :clean? false}
             {:rotten? false :clean? false}]
    :color :black}])
//}

#@# We are interested in splitting the grape clusters into individual grapes, discarding the rotten ones and cleaning the remaining grapes so they are ready for eating. We are well-equipped in ClojureScript for this data transformation task; we could implement it using the familiar  map ,  filter  and  mapcat  functions:
私たちは、ブドウの房を個々のブドウに分け、腐ったものを捨て、残りのブドウを食べられるようにきれいにしようとしています。このデータ変換の作業を行うための装備が ClojureScript には十分に備わっています。おなじみの @<code>{map} 、@<code>{filter}、@<code>{mapcat} 関数を使って実装できます。

//emlist{
(defn split-cluster
  [c]
  (:grapes c))

(defn not-rotten
  [g]
  (not (:rotten? g)))

(defn clean-grape
  [g]
  (assoc g :clean? true))
//}


#@# Page104
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(->> grape-clusters
     (mapcat split-cluster)
     (filter not-rotten)
     (map clean-grape))
     ;; => ({rotten? false :clean? true}
     ;;     {:rotten? false :clean? true})
//}

#@# In the above example we succintly solved the problem of selecting and cleaning the grapes, and we can even abstract such transformations by combining the  mapcat ,  filter  and  map  operations using partial application and function composition:
上記の例では、ブドウの選択と洗浄の問題を上手に解決していますが、partial application と 関数合成を使いながら @<code>{mapcat}、@<code>{filter}、@<code>{map} を組み合わせることによって、このような変換を抽象化することもできます。

//emlist{
(def process-clusters
  (comp
   (partial map clean-grape)
   (partial filter not-rotten)
   (partial mapcat split-cluster)))

(process-clusters grape-clusters)
;; => ({rotten? false :clean? true}
;;     {:rotten? false :clean? true})
//}

#@# The code is very clean, but it has a few problems. For example, each call to  map ,  filter  and  mapcat  consumes and produces a sequence that, although lazy, generates intermediate results that will be discarded. Each sequence is fed to the next step, which also returns a sequence. Wouldn't be great if we could do the transformation in a single transversal of the  grape-cluster  collection?
コードは非常にきれいですが、いくつか問題があります。たとえば、@<code>{map} @<code>{filter} @<code>{mapcat} の各呼び出しは、遅延はあっても破棄される中間結果を生成するシーケンスを消費して生成します。各シーケンスは次のステップに渡され、シーケンスを返します。ブドウの房のコレクションを1回の横断で変換できるとしたら素晴らしいと思いませんか。

#@# Another problem is that even though our  process-clusters  function works with any sequence, we can't reuse it with anything that is not a sequence. Imagine that instead of having the grape cluster collection available in memory it is being pushed to us asynchronously in a stream. In that situation we couldn't reuse  process-clusters  since usually  map ,  filter  and  mapcat  have concrete implementations depending on the type.
@<code>{process-clusters} 関数は、どんなシーケンスにも機能しますが、シーケンス以外には再利用できないことも問題です。ブドウの房のコレクションをメモリ上で利用可能にするのではなく、ストリームの中で非同期にプッシュされることを想像してみてください。このような状況では、@<code>{map} @<code>{filter} @<code>{mapcat} などは、型に対応した具体的な実装があるため、@<code>{process-clusters} 関数を再利用できなかったのです。

=== プロセス変換への一般化

#@# Generalizing to process transformations

#@# The process of mapping, filtering or mapcatting isn't necessarily tied to a concrete type, but we keep reimplementing them for different types. Let's see how we can generalize such processes to be context independent. We'll start by implementing naive versions of  map  and  filter  first to see how they work internally:
map filter mapcat を行うプロセスは、必ずしも具体的な型に結び付けられているわけではありませんが、各々の型に対して再実装していきます。文脈に依存しないように、このようなプロセスを一般化する方法を見てみましょう。まず、@<code>{map} と @<code>{filter} の単純なバージョンを実装して、内部でどのように動作するかを見てみましょう。


#@# Page105
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(defn my-map
  [f coll]
  (when-let [s (seq coll)]
    (cons (f (first s)) (my-map f (rest s)))))

(my-map inc [0 1 2])
;; => (1 2 3)

(defn my-filter
  [pred coll]
  (when-let [s (seq coll)]
    (let [f (first s)
          r (rest s)]
      (if (pred f)
        (cons f (my-filter pred r))
        (my-filter pred r)))))

(my-filter odd? [0 1 2])
;; => (1)
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# As we can see, they both assume that they receive a seqable and return a sequence. Like many recursive functions they can be implemented in terms of the already familiar  reduce  function. Note that functions that are given to reduce receive an accumulator and an input and return the next accumulator. We'll call these types of functions reducing functions from now on.
どちらも seqable を受け取り、シーケンスを返すことを前提としています。多くの再帰関数と同様に、これらはおなじみの @<code>{reduce} 関数として実装できます。@<code>{reduce} 関数はアキュムレータと入力を受け取り、次のアキュムレータを返します。今後は、このような関数を reducing 関数(reduce を行う関数) と呼びます。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//emlist{
(defn my-mapr
  [f coll]
  ;; reducing function
  (reduce (fn [acc input]
            (conj acc (f input)))
          ;; initial value
          []
          ;; collection to reduce
          coll))

(my-mapr inc [0 1 2])
;; => [1 2 3]

(defn my-filterr
  [pred coll]
  ;; reducing function
  (reduce (fn [acc input]
            (if (pred input)
              (conj acc input)
              acc))
          ;; initial value
          []
          ;; collection to reduce
          coll))

(my-filterr odd? [0 1 2])
;; => [1]
//}


#@# Page106
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# We've made the previous versions more general since using  reduce  makes our functions work on any thing that is reducible, not just sequences. However you may have noticed that, even though  my-mapr  and  my-filterr  don't know anything about the source ( coll ) they are still tied to the output they produce (a vector) both with the initial value of the reduce ( [] ) and the hardcoded  conj  operation in the body of the reducing function. We could have accumulated results in another data structure, for example a lazy sequence, but we'd have to rewrite the functions in order to do so.
@<code>{reduce} を使用すると、シーケンスだけでなく、還元可能なあらゆるものに対して関数が動作するようになるので、以前のバージョンはより一般的なものになりました。しかし、@<code>{my-mapr} と @<code>{my-filterr} はソース (coll) について何も知らないにもかかわらず、 @<code>{reduce} の初期値 ( [] ) と  @<code>{reduce} 関数の本体でハードコードされた @<code>{conj} 演算によって、出力(ベクタ)と結びついていることに気づかれたかも知れません。結果を別のデータ構造、例えば遅延シーケンスに蓄積することもできますが、そのためには関数を書き直さなければなりません。

#@# How can we make these functions truly generic? They shouldn't know about either the source of inputs they are transforming nor the output that is generated. Have you noticed that  conj  is just another reducing function? It takes an accumulator and an input and returns another accumulator. So, if we parameterise the reducing function that  my-mapr  and  my-filterr  use, they won't know anything about the type of the result they are building. Let's give it a shot:
これらの関数を本当の意味で汎用的にするにはどうすればよいのでしょうか。これらの関数は、変換中の入力のソースや、生成された出力について知るべきではありません。@<code>{conj} は reducing 関数の1つであることにお気づきでしょうか。アキュムレータと入力を受け取り、別のアキュムレータを返します。したがって、@<code>{my-mapr} と @<code>{my-filterr} が使用する reducing 関数をパラメータ化しても、ビルドする結果の型については何も知りません。試してみましょう。

//emlist{
(defn my-mapt
  ;; function to map over inputs
  [f]
  ;; parameterised reducing function
  (fn [rfn]
    ;; transformed reducing function, now it maps  f !
    (fn [acc input]
      (rfn acc (f input)))))

(def incer (my-mapt inc))

(reduce (incer conj) [] [0 1 2])
;; => [1 2 3]

(defn my-filtert
  ;; predicate to filter out inputs
  [pred]
  ;; parameterised reducing function
  (fn [rfn]
    ;; transformed reducing function,
    ;; now it discards values based on  pred !
    (fn [acc input]
      (if (pred input)
        (rfn acc input)
        acc))))

(def only-odds (my-filtert odd?))

(reduce (only-odds conj) [] [0 1 2])
;; => [1]
//}

#@# That's a lot of higher-order functions so let's break it down for a better understanding of what's going on. We'll examine how  my-mapt  works step by step. The mechanics are similar for  my-filtert , so we'll leave it out for now.
高階関数が多いので、何が起こっているのかを理解するために分解してみましょう。@<code>{my-mapt} がどのように機能するかを段階的に検証していきます。@<code>{my-filtert} の仕組みは似ているので、ここでは省略します。

#@# First of all,  my-mapt  takes a mapping function; in the example we are giving it  inc  and getting another function back. Let's replace  f  with  inc  to see what we are building:
まず、@<code>{my-mapt}　はマッピング関数を取ります。この例では、@<code>{inc} を指定して別の関数を返しています。@<code>{f} を @<code>{inc} に置き換えて、build しているものを確認しましょう。


#@# Page107
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def incer (my-mapt inc))
;; (fn [rfn]
;;   (fn [acc input]
;;     (rfn acc (inc input))))
;;               ^^^
//}

#@# The resulting function is still parameterised to receive a reducing function to which it will delegate, let's see what happens when we call it with  conj :

@<embed>{|latex|\vspace{-0.3\Cvs\}}

結果として得られる関数は、それがデリゲートする reducing 関数を受け取るようにパラメータ化されています。それを cond と一緒に呼んだときに何が起こるか見てみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(incer conj)
;; (fn [acc input]
;;   (conj acc (inc input)))
;;    ^^^^
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# We get back a reducing function which uses  inc  to transform the inputs and the  conj  reducing function to accumulate the results. In essence, we have defined map as the transformation of a reducing function.  The functions that transform one reducing function into another are called transducers in ClojureScript.
入力の変換のために @<code>{inc} 、結果を蓄積するために @<code>{conj} を使用する reducing 関数を取り戻します。本質的に、reducing  関数の変換として map 定義しました。ある reducing 関数を別の reducing 関数に変換する関数のことを ClojureScript では transducers と呼びます。

#@# To ilustrate the generality of transducers, let's use different sources and destinations in our call to  reduce :
transducer の一般性を示すために、reduce の呼び出しにおいて、異なるソースと出力先を使用してみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(reduce (incer str) "" [0 1 2])
;; => "123"

(reduce (only-odds str) "" '(0 1 2))
;; => "1"
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# The transducer versions of  map  and  filter  transform a process that carries inputs from a source to a destination but don't know anything about where the inputs come from and where they end up. In their implementation they contain the _essence_ of what they accomplish, independent of context.
@<code>{map} 及び @<code>{filter} の transducer版は、入力をソースから出力先に運ぶプロセスを変換しますが、入力がどこから来てどこで終わるかは知りません。これらの実装においては、文脈に関係なく目的を達成することの本質が含まれています。

#@# Now that we know more about transducers we can try to implement our own version of  mapcat . We already have a fundamental piece of it: the  map  transducer. What  mapcat  does is map a function over an input and flatten the resulting structure one level. Let's try to implement the catenation part as a transducer:
transducer についての知識が増えたので、独自のバージョンの @<code>{mapcat} を実装できます。私たちはすでにその基本的な部分を持っています。@<code>{map} transducer です。@<code>{mapcat} が行うことは、関数を入力上にマップし、結果の構造を1レベルの平らさにすることです。結合する箇所を transducer として実装してみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn my-cat
  [rfn]
  (fn [acc input]
    (reduce rfn acc input)))

(reduce (my-cat conj) [] [[0 1 2] [3 4 5]])
;; => [0 1 2 3 4 5]
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}


#@# Page108
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


#@# The  my-cat  transducer returns a reducing function that catenates its inputs into the accumulator. It does so reducing the  input  reducible with the  rfn  reducing function and using the accumulator ( acc ) as the initial value for such reduction.  mapcat  is simply the composition of  map  and  cat . The order in which transducers are composed may seem backwards but it'll become clear in a moment.
my-cat transducer は、その入力をアキュムレータに結合する reducing 関数を返します。このようにして、rfn 関数で reduce 可能な入力を reduce して、このような reduce のための初期値としてアキュムレータ(acc)を使用します。mapcat は map と cat の合成です。transducer が構成される順序は逆向きに見えるかもしれませんが、この点については後に明らかにします。

//emlist{
(defn my-mapcat
  [f]
  (comp (my-mapt f) my-cat))

(defn dupe
  [x]
  [x x])

(def duper (my-mapcat dupe))

(reduce (duper conj) [] [0 1 2])
;; => [0 0 1 1 2 2]
//}


=== ClojureScript core における Transducer

#@# Transducers in ClojureScript core

#@# Some of the ClojureScript core functions like  map ,  filter  and  mapcat  support an arity 1 version that returns a transducer. Let's revisit our definition of  process-cluster  and define it in terms of transducers:
@<code>{map} @<code>{filter} @<code>{mapcat} のような ClojureScript の core 関数は、transducer を返す 1 バージョンの項数をサポートします。@<code>{process-cluster} の定義を再考して、transducer の観点から定義してみましょう。

//emlist{
(def process-clusters
  (comp
   (mapcat split-cluster)
   (filter not-rotten)
   (map clean-grape)))
//}

#@# A few things changed since our previous definition  process-clusters . First of all, we are using the transducer-returning versions of  mapcat ,  filter  and  map  instead of partially applying them for working on sequences.
前の @<code>{process-clusters} の定義からいくつかの点が変わりました。まず、シーケンスの処理に部分的に適用する代わりに、transducer を返すバージョンの @<code>{mapcat} @<code>{filter} @<code>{map} を使用します。

#@# Also you may have noticed that the order in which they are composed is reversed, they appear in the order they are executed. Note that all  map ,  filter  and  mapcat  return a transducer.  filter  transforms the reducing function returned by  map , applying the filtering before proceeding;  mapcat  transforms the reducing function returned by  filter , applying the mapping and catenation before proceeding.
合成される順序が逆であり、実行された順序で表示されることにお気づきでしょうか。@<code>{map}、@<code>{filter}、@<code>{mapcat} は全て transducer を返すことに注目してください。@<code>{filter} は @<code>{map} が返す reducing 関数を変換して、次に進む前にフィルタリングを適用します。@<code>{mapcat} は @<code>{filter} が返す reducing 関数を変換して、マッピングと結合を適用してから処理を進めます。

#@# One of the powerful properties of transducers is that they are combined using regular function composition.  What's even more elegant is that the composition of various transducers is itself a transducer! This means that our  process-cluster  is a transducer too, so we have defined a composable and context-independent algorithmic transformation.
transducer の強力な特性の一つは、それらが規則的な関数合成を用いて合成されることです。さらにエレガントなのは、様々な transducer の合成そのものが transducer であることです。これは、定義した @<code>{process-cluster} も transducer であることを意味するので、合成可能で文脈に依存しないアルゴリズムの変換を定義したことになります。

#@# Page109
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

#@# Many of the core ClojureScript functions accept a transducer, let's look at some examples with our newly created  process-cluster :
ClojureScript の core 関数の多くは transducer を受けとりますが、新しく作成した process-cluster でいくつか例を見てみましょう。

//emlist{
(into [] process-clusters grape-clusters)
;; => [{:rotten? false, :clean? true}
;;     {:rotten? false, :clean? true}]

(sequence process-clusters grape-clusters)
;; => ({:rotten? false, :clean? true} {:rotten? false, :clean? true})

(reduce (process-clusters conj) [] grape-clusters)
;; => [{:rotten? false, :clean? true} {:rotten? false, :clean? true}]
//}

#@# Since using  reduce  with the reducing function returned from a transducer is so common, there is a function for reducing with a transformation called  transduce . We can now rewrite the previous call to  reduce  using  transduce :
transducer から返される reducing 関数を用いて @<code>{reduce} を使用することは非常に一般的であるため、transduce と呼ばれる変換(transformation)で reduce を行う関数が存在します。これで、reduce への前の呼び出しを @<code>{transduce} を用いて書き直すことができます。

//emlist{
(transduce process-clusters conj [] grape-clusters)
;; => [{:rotten? false, :clean? true} {:rotten? false, :clean? true}]
//}


=== 初期化(Initialisation)

#@# Initialisation

#@# In the last example we provided an initial value to the  transduce  function ( [] ) but we can omit it and get the same result:
最後の例では、@<code>{transduce} 関数(@<code>{[]})に初期値を指定しましたが、省略しても同じ結果が得られます。

//emlist{
(transduce process-clusters conj grape-clusters)
;; => [{:rotten? false, :clean? true} {:rotten? false, :clean? true}]
//}

#@# What's going on here? How can  transduce  know what initial value use as an accumulator when we haven't specified it? Try calling  conj  without any arguments and see what happens:
何が起こっているのでしょうか。アキュムレータとして使用する初期値を指定していないのに、その値が何であるかを @<code>{transduce} はどのようにして知ることができるのでしょうか。引数なしで @<code>{conj} を呼び出して何が起こるかを確認してみます。

//emlist{
(conj)
;; => []
//}

#@# The  conj  function has a arity 0 version that returns an empty vector but is not the only reducing function that supports arity 0. Let's explore some others:
@<code>{conj} 関数には空のベクタを返す引数が必要ないバージョンがありますが、これだけが引数なしを許す reducing 関数という訳ではありません。他の関数も見てみましょう。


#@# Page110
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(+)
;; => 0

(*)
;; => 1

(str)
;; => ""

(= identity (comp))
;; => true
//}

#@# The reducing function returned by transducers must support the arity 0 as well, which will typically delegate to the transformed reducing function. There is no sensible implementation of the arity 0 for the transducers we have implemented so far, so we'll simply call the reducing function without arguments. Here's how our modified  my-mapt  could look like:
transducer によって返される reducing 関数は、同様に引数がない場合をサポートしなければならず、これは通常、変換された reducing 関数にデリゲートされます。これまでに実装した transducer には、引数なし版の実用的な実装はありませんでした。引数なし版で単に reducing 関数を呼び出してみます。修正した my-mapt は以下のようになります。

//emlist{
(defn my-mapt
  [f]
  (fn [rfn]
    (fn
      ;; arity 0 that delegates to the reducing fn
      ([] (rfn))
      ([acc input]
       (rfn acc (f input))))))
//}

#@# The call to the arity 0 of the reducing function returned by a transducer will call the arity 0 version of every nested reducing function, eventually calling the outermost reducing function. Let's see an example with our already defined  process-clusters  transducer:
transducer によって返される reducing 関数の引数なし版の呼び出しは、ネストされたすべての reducing 関数の引数なし版を呼び出し、最終的に最も外側の reducing 関数を呼び出します。定義済みの process-clusters transducer の例を見てみましょう。

//emlist{
((process-clusters conj))
;; => []
//}

#@# The call to the arity 0 flows through the transducer stack, eventually calling  (conj) .
引数がない場合の呼び出しは transducer のスタックを経由して、最終的に @<code>{(conj)} を呼び出します。

=== ステートフルな Transducer

#@# So far we've only seen purely functional transducer; they don't have any implicit state and are very predictable. However, there are many data transformation functions that are inherently stateful, like  take .  take  receives a number  n  of elements to keep and a collection and returns a collection with at most  n  elements.

ここまでは純粋に関数的な transducer だけを見てきました。つまり、暗黙的な状態を持たず、状態の予測は容易です。しかし、take のような本質的に状態をもつデータを変換する関数が多くあります。take は保持する n 個の要素と 1 つのコレクションを受け取り、最大で n 個の要素を持つコレクションを返します。


#@# Page111
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(take 10 (range 100))
;; => (0 1 2 3 4 5 6 7 8 9)
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Let's step back for a bit and learn about the early termination of the  reduce function. We can wrap an accumulator in a type called  reduced  for telling  reduce  that the reduction process should terminate immediately. Let's see an example of a reduction that aggregates the inputs in a collection and finishes as soon as there are 10 elements in the accumulator:
少し話題を変えて、reduce 関数の早期終了について学びましょう。アキュムレータを reduced と呼ばれる型にラップして、reduce の処理をすぐに終了するように指示することができます。コレクション内の入力を集約し、アキュムレータに 10 個の要素が入ると終了する reduce の例を見てみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(reduce (fn [acc input]
          (if (= (count acc) 10)
            (reduced acc)
            (conj acc input)))
        []
        (range 100))
;; => [0 1 2 3 4 5 6 7 8 9]
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Since transducers are modifications of reducing functions they also use  reduced  for early termination.  Note that stateful transducers may need to do some cleanup before the process terminates, so they must support an arity 1 as a "completion" step. Usually, like with arity 0, this arity will simply delegate to the transformed reducing function's arity 1.
transducer は reducing 関数の変形であるため、早期終了のために reduced を使用します。状態をもつ transducer はプロセスが終了する前に、何らかの掃除を行う必要があるかもしれないので、完成のためのステップとして、引数を1つとる場合をサポートしなければならないことに注意してください。通常、引数を取らない場合と同様に引数は、引数が 1 つである変形された reducing 関数にデリゲートされます。

#@# Knowing this we are able to write stateful transducers like  take . We'll be using mutable state internally for tracking the number of inputs we have seen so far, and wrap the accumulator in a  reduced  as soon as we've seen enough elements:
これを知っていると、take のような状態をもつ transducer を書くことができます。これまでに見てきたような入力の数を追跡するために、可変な状態を内部的に使用して、必要な要素数を確認したらすぐに、アキュムレータを reduced にラップします。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn my-take
  [n]
  (fn [rfn]
    (let [remaining (volatile! n)]
      (fn
        ([] (rfn))
        ([acc] (rfn acc))
        ([acc input]
         (let [rem @remaining
               nr (vswap! remaining dec)
               ;; we still have items to take
               result (if (pos? rem)
                        ;; we're done,
                        ;; acc becomes the result
                        (rfn acc input)
                        acc)]
           ;; wrap result in reduced if not already
           (if (not (pos? nr))
             (ensure-reduced result)
             result)))))))
//}


#@# Page112
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# This is a simplified version of the  take  function present in ClojureScript core. There are a few things to note here so let's break it up in pieces to understand it better.
これは ClojureScript の core にある @<code>{take} 関数を単純化したものです。注意すべき点がいくつかあるので、少しずつ分けて見ていきましょう。

#@# The first thing to notice is that we are creating a mutable value inside the transducer. Note that we don't create it until we receive a reducing function to transform. If we created it before returning the transducer we couldn't use  my-take  more than once. Since the transducer is handed a reducing function to transform each time it is used, we can use it multiple times and the mutable variable will be created in every use.
まず最初に注意しなければならないのは、transducer 内で変更可能な値を作成している点です。変換を行う reducing 関数を受け取るまでは値を作成していないことに注意してください。transducer を返す前に作ってしまうと、my-take 関数を 2 回以上使うことはできません。transducer は、使用の度に変換を行う reducing 関数を渡されるので、何度でも使うことができます。また、変更可能な変数は、毎回使うたびに作成されます。

//emlist{
(fn [rfn]
  ;; make sure to create mutable variables
  ;; inside the transducer  
  (let [remaining (volatile! n)]
    (fn
      ;; ...
)))

(def take-five (my-take 5))

(transduce take-five conj (range 100))
;; => [0 1 2 3 4]

(transduce take-five conj (range 100))
;; => [0 1 2 3 4]
//}

#@# Let's now dig into the reducing function returned from  my-take . First of all we  deref  the volatile to get the number of elements that remain to be taken and decrement it to get the next remaining value.  If there are still remaining items to take, we call  rfn  passing the accumulator and input; if not, we already have the final result.
ここで、@<code>{my-take} から返される reducing 関数について詳しく見ていきましょう。まず volatile に @<code>{deref} を行い、取得されるべき残りの要素の数を取得して、それをデクリメントして次の残りの値を取得します。もし、まだ取るべきアイテムが残っているなら、アキュムレータと入力を渡して @<code>{rfn} を呼びます。そうでなければ、すでに最終的な結果をもっています。

//emlist{
([acc input]
  (let [rem @remaining
        nr (vswap! remaining dec)
        result (if (pos? rem)
                 (rfn acc input)
                 acc)]
    ;; ...
))
//}

#@# The body of  my-take  should be obvious by now. We check whether there are still items to be processed using the next remainder ( nr ) and, if not, wrap the result in a  reduced  using the  ensure-reduced  function.  ensure-reduced  will wrap the value in a  reduced  if it's not reduced already or simply return the value if it's already reduced. In case we are not done yet, we return the accumulated  result  for further processing.
@<code>{my-take} の本体は明らかになっているはずです。次の剰余(nr)を使用して、処理されるアイテムがまだ存在するかどうかをチェックします。存在しない場合は、ensure-reduced 関数を使用して、結果を reduced にラップします。ensure-reduced は、まだ reduce されていない場合は、値を reduced でラップして、すでに削減されている場合には単に値を返します。まだ完了していない場合は、累積した結果を返して処理します。


#@# Page113
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(if (not (pos? nr))
  (ensure-reduced result)
  result)
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# We've seen an example of a stateful transducer but it didn't do anything in its completion step. Let's see an example of a transducer that uses the completion step to flush an accumulated value. We'll implement a simplified version of  partition-all , which given a  n  number of elements converts the inputs in vectors of size  n . For understanding its purpose better let's see what the arity 2 version gives us when providing a number and a collection:
ステートフルな transducer の例を見てきましたが、完了の段階では何もしませんでした。累積値を表示するために完了のステップを使用する transducer の例を見てみましょう。要素の数が n の要素あれば、サイズが n 個のベクタの入力を変換する @<code>{partition-all} の簡素版を実装しましょう。目的をよく理解するために、引数を 2 つとる場合で、数字とコレクションを与えると何が得られるかを見てみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(partition-all 3 (range 10))
;; => ((0 1 2) (3 4 5) (6 7 8) (9))
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# The transducer returning function of  partition-all  will take a number  n  and return a transducer that groups inputs in vectors of size  n . In the completion step it will check if there is an accumulated result and, if so, add it to the result. Here's a simplified version of ClojureScript core  partition-all  function, where  array-list  is a wrapper for a mutable JavaScript array:
@<code>{partition-all} の transducer を返す関数は、数値 @<code>{n} を取り、@<code>{n} 個のサイズのベクタで入力をグループ化する transducer を返しまる。完了する段階では、累積結果があるかどうかを確認して、ある場合は結果に追加します。以下は、ClojureScript の core 関数である @<code>{partition-all} を単純化したもので、@<code>{array-list} は変更可能な JavaScript の配列のラッパーです。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(defn my-partition-all
  [n]
  (fn [rfn]
    (let [a (array-list)]
      (fn
        ([] (rfn))
        ([result]
         ;; no inputs accumulated, don't have to modify result
         (let [result (if (.isEmpty a)
                        result
                        (let [v (vec (.toArray a))]
                          ;; flush array contents for garbage collection
                          (.clear a)
                          ;; pass to  rfn , removing the reduced wrapper if present
                          (unreduced (rfn result v))))]
           (rfn result)))
        ([acc input]
         (.add a input)
         ;; got enough results for a chunk
         (if (== n (.size a))
           (let [v (vec (.toArray a))]
             (.clear a)
             ;; the accumulated chunk
             ;; becomes input to  rfn
             (rfn acc v))
           acc))))))

//}


#@# Page114
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def triples (my-partition-all 3))

(transduce triples conj (range 10))
;; => [[0 1 2] [3 4 5] [6 7 8] [9]]
//}


=== Eduction

#@# Eductions are a way to combine a collection and one or more transformations that can be reduced and iterated over, and that apply the transformations every time we do so. If we have a collection that we want to process and a transformation over it that we want others to extend, we can hand them a eduction, encapsulating the source collection and our transformation. We can create an eduction with the  eduction  function:
Eduction とは、コレクションと 1 つ以上の変換を組み合わせる方法であり、それらに対して reduce したり、繰り返したりすることができ、そのたびに変換を適用します。処理したいコレクションとそれに対する変換があり、他に拡張してほしい場合、私たちはソースコレクションと私たちの変換をカプセル化したEductionをそれらに渡すことができます。eduction 関数を使用して eduction を作成できます。

//emlist{
(def ed (eduction (filter odd?) (take 5) (range 100)))

(reduce + 0 ed)
;; => 25

(transduce (partition-all 2) conj ed)
;; => [[1 3] [5 7] [9]]
//}


=== ClojureScript core における他の Transducer 

#@# More transducers in ClojureScript core

#@# We learned about  map ,  filter ,  mapcat ,  take  and  partition-all , but there are a lot more transducers available in ClojureScript. Here is an incomplete list of some other intersting ones:
@<code>{map} 、@<code>{filter} 、@<code>{mapcat} 、@<code>{take} 、@<code>{partition-all} について学びましたが、ClojureScript にはもっと多くの transducer があります。以下は、他の興味深いものの一部です。

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# -  drop  is the dual of  take , dropping up to  n  values before passing inputs to the reducing function
#@# -  distinct  only allows inputs to occur once
#@# -  dedupe  removes succesive duplicates in input values

- @<code>{drop} は take の双対であり、入力を reducing 関数に渡す前に @<code>{n} の値まで小さくします

- @<code>{distinct} は、入力を 1 回だけ行うことを許可します。

- @<code>{dedupe} は入力値の連続する重複を削除します

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@# We encourage you to explore ClojureScript core to see what other transducers are out there.
他にどんな transducer があるのかを知るために、ClojureScript core を調べてみることをお勧めします。

=== Transducer を定義する

#@# Defining our own transducers

#@# There a few things to consider before writing our own transducers so in this section we will learn how to properly implement one. First of all, we've learned that the general structure of a transducer is the following:
独自の transducer を作成する前に考慮すべき点がいくつかあるので、この章では、transducer を適切に実装する方法を学びます。まず、transducer の一般的な構造は次の通りであることを学びました。


#@# Page115
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(fn [xf]
  (fn
    ;; init
    ([]
     ...)
    ;; completion
    ([r]
     ...)
    ;; step
    ([acc input]
     ...)))
//}

#@# Usually only the code represented with ...  changes between transducers, these are the invariants that must be preserved in each arity of the resulting function:
通常、transducer の間では ... の箇所のコードのみが変化します。これらは、結果として得られる関数のそれぞれの引数で保存されなければならない不変の式です。

@<embed>{|latex|\vspace{1\Cvs\}}

#@# * arity 0 (init): must call the arity 0 of the nested transform  xf
#@#  * arity 1 (completion): used to produce a final value and potentially flush state, must call the arity 1 of the nested transform  xf  *exactly once*
#@#  * arity 2 (step): the resulting reducing function which will call the arity 2 of the nested transform  xf  zero, one or more times

- 0 番目の引数(init): ネストされた変換 @<code>{xf} の 0 番目の引数を呼び出す必要があります。

- 1 番目の引数(completion): 最終値を生成し、潜在的にフラッシュ状態を生成するために使用され、ネストされたトランスフォーム @<code>{xf} の 1 番目の引数を正確に 1 度だけ呼び出す必要があります。

- 2 番目の引数(ステップ): 結果を導く reducing 関数であり、ネストされたトランスフォーム @<code>{xf} の 2 番目の引数を 0 回以上呼び出します。

=== トランスデゥシブルなプロセス

#@# Transducible processes

#@# A transducible process is any process that is defined in terms of a succession of steps ingesting input values.  The source of input varies from one process to another. Most of our examples dealt with inputs from a collection or a lazy sequence, but it could be an asynchronous stream of values or a  core.async  channel. The outputs produced by each step are also different for every process;  into  creates a collection with every output of the transducer,  sequence  yields a lazy sequence, and asynchronous streams would probably push the outputs to their listeners.
トランスデゥシブルなプロセスとは、入力値を取り込む一連のステップによって定義される任意のプロセスです。入力のソースは、プロセスによって異なります。私たちの例の大半は、コレクションまたは遅延シーケンスからの入力を扱っていますが、非同期の値のストリーム、または core.async のチャンネルである可能性があります。各ステップで生成される出力もプロセスごとに異なります。into は transducer のすべての出力を持つコレクションを作成して、シーケンスは遅延シーケンスを生成し、非同期ストリームは出力をリスナーに通知します。

#@# In order to improve our understanding of transducible processes, we're going to implement an unbounded queue, since adding values to a queue can be thought in terms of a succession of steps ingesting input. First of all we'll define a protocol and a data type that implements the unbounded queue:
変換可能なプロセスについての理解を深めるために、無限の列(unbounded queue)を実装します。列に値を追加することは、入力を取り込む一連のステップと捉えることができるため、まず、無限の列を実装するプロトコルとデータ型を定義します。

//emlist{
(defprotocol Queue
  (put! [q item] "put an item into the queue")
  (take! [q] "take an item from the queue")
  (shutdown! [q] "stop accepting puts in the queue"))
//}

#@# Page116
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

//emlist{
(deftype UnboundedQueue [^:mutable arr ^:mutable closed]
  Queue
  (put! [_ item]
    (assert (not closed))
    (assert (not (nil? item)))
    (.push arr item)
    item)
  (take! [_]
    (aget (.splice arr 0 1) 0))
  (shutdown! [_]
    (set! closed true)))
//}

#@# We defined the  Queue  protocol and as you may have noticed the implementation of  UnboundedQueue  doesn't know about transducers at all. It has a  put!  operation as its step function and we're going to implement the transducible process on top of that interface:
私たちは @<code>{Queue} プロトコルを定義しましたが、お気付きかもしれませんが、@<code>{UnboundedQueue} の実装は transducer をまったく認識しません。そのステップ関数として @<code>{put!} があるので、このインターフェースの上に transducible プロセスを実装します。

//emlist{
(defn unbounded-queue
  ([]
   (unbounded-queue nil))
  ([xform]
   (let [put! (completing put!)
         xput! (if xform (xform put!) put!)
         q (UnboundedQueue. #js [] false)]
     (reify
       Queue
       (put! [_ item]
         (when-not (.-closed q)
           (let [val (xput! q item)]
             (if (reduced? val)
               (do
                 ;; call completion step
                 (xput! @val)
                 ;; respect reduced
                 (shutdown! q)
                 @val)
               val))))
       (take! [_]
         (take! q))
       (shutdown! [_]
         (shutdown! q))))))
//}

#@# As you can see, the  unbounded-queue  constructor uses an  UnboundedQueue  instance internally, proxying the  take!  and  shutdown!  calls and implementing the transducible process logic in the  put!  function. Let's deconstruct it to understand what's going on.
@<code>{unbounded-queue} コンストラクタは、内部的に @<code>{UnboundedQueue} インスタンスを使用して、@<code>{take!} と @<code>{shutdown!} の呼び出しをプロキシして、@<code>{put!} 関数の中のトランスデゥシブルなロジックを実装します。何が起こっているのか理解するために、少しずつ見ていきましょう。


#@# Page117
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}

//emlist{
(let [put! (completing put!)
      xput! (if xform (xform put!) put!)
      q (UnboundedQueue. #js [] false)]
  ;; ...
)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# First of all, we use  completing  for adding the arity 0 and arity 1 to the  Queue  protocol's  put!  function.  This will make it play nicely with transducers in case we give this reducing function to  xform  to derive another. After that, if a transducer ( xform ) was provided, we derive a reducing function applying the transducer to  put! .  If we're not handed a transducer we will just use  put! .  q  is the internal instance of  UnboundedQueue .
まず、Queue プロトコルの put! 関数に、引数をとらない場合と引数を 1 つとる場合を追加するために @<code>{completing} を使います。これにより、この reducing 関数を @<code>{xform} に与えて別のものを引き出す場合に、transducer とうまく動作するようになります。その後、もし transducer(@<code>{xform})が与えられると、transducer を put! に適用しながら reducing 関数を引き出します。transducer が渡されなければ、@<code>{put!} を使います。@<code>{q} は @<code>{UnboundedQueue} の内部インスタンスです。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(reify
  Queue
  (put! [_ item]
    (when-not (.-closed q)
      (let [val (xput! q item)]
        (if (reduced? val)
          (do
            ;; call completion step
            (xput! @val)
            ;; respect reduced
            (shutdown! q)
            @val)
          val))))
  ;; ...
)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}


#@# The exposed  put!  operation will only be performed if the queue hasn't been shut down. Notice that the  put!  implementation of  UnboundedQueue  uses an assert to verify that we can still put values to it and we don't want to break that invariant. If the queue isn't closed we can put values into it, we use the possibly transformed  xput!  for doing so.
expose された @<code>{put!} 操作は queue がシャットダウンされていない場合にのみ実行されます。UnboundedQueue の @<code>{put!} の実装では、新たな値を入れることができるか、不変性を壊さないかを検証するために assert を用います。もし queue が閉じられておらず、値を入れることができる場合は、変換される可能性がある @<code>{xput!} を用います。

#@# If the put operation gives back a reduced value it's telling us that we should terminate the transducible process.  In this case that means shutting down the queue to not accept more values. If we didn't get a reduced value we can happily continue accepting puts.
put の操作が reduce された値を返した場合、transducible なプロセスを終了する必要があることを示しています。この場合、queue をシャットダウンして、追加の値を受け入れないようにします。もし reduce された値をえられなければ、puts を引き受け続けることができます。

#@# Let's see how our queue behaves without transducers:
queue が transducer がない状態でどう動作するかを見てみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def q (unbounded-queue))
;; => #<[object Object]>

(put! q 1)
;; => 1
(put! q 2)
;; => 2
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page118
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(take! q)
;; => 1
(take! q)
;; => 2
(take! q)
;; => nil
//}

#@# Pretty much what we expected, let's now try with a stateless transducer:
期待していた通りですね。今度はステートレスな transducer で試してみましょう。

//emlist{
(def incq (unbounded-queue (map inc)))
;; => #<[object Object]>

(put! incq 1)
;; => 2
(put! incq 2)
;; => 3

(take! incq)
;; => 2
(take! incq)
;; => 3
(take! incq)
;; => nil
//}

#@# To check that we've implemented the transducible process, let's use a stateful transducer. We'll use a transducer that will accept values while they aren't equal to 4 and will partition inputs in chunks of 2 elements:
transducible なプロセスを実装したかどうかを確認するために、ステートフルな transducer を使いましょう。4 と等しくない間は値を受け入れ、入力を2つの要素のチャンクに分割する transducer を使用します。

//emlist{
(def xq (unbounded-queue (comp
                           (take-while #(not= % 4))
                           (partition-all 2))))

(put! xq 1)
(put! xq 2)
;; => [1 2]
(put! xq 3)
(put! xq 4) ;; shouldn't accept more values from here on
(put! xq 5)
;; => nil

(take! xq)
;; => [1 2]
;; seems like partition-all flushed correctly!
(take! xq) 
;; => [3]
(take! xq)
;; => nil
//}


#@# Page119
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

#@# The example of the queue was heavily inspired by how  core.async  channels use transducers in their internal step. We'll discuss channels and their usage with transducers in a later section.
queue の例は、@<code>{core.async} チャンネルが内部ステップで transducer を使う方法にに大きく影響を受けています。チャンネルと transducer を使ったチャンネルの使い方については、後のセクションで説明します。

#@# Transducible processes must respect  reduced  as a way for signaling early termination. For example, building a collection stops when encountering a  reduced  and  core.async  channels with transducers are closed.  The  reduced  value must be unwrapped with  deref  and passed to the completion step, which must be called exactly once.
Transducible なプロセスは、早期終了のシグナルの伝達手段として @<code>{reduced} を反映しなければいけません。たとえば、コレクションのビルドは、@<code>{reduced} に遭遇したときや、transducer をもつ @<code>{core.async} チャンネルが閉じられた場合に停止します。reduce された値は必ず @<code>{deref} で開封されて、完了のステップに渡されます。完了ステップは 1 度しか呼び出されてはいけません。

#@# Transducible processes shouldn't expose the reducing function they generate when calling the transducer with their own step function since it may be stateful and unsafe to use from elsewhere.
transducible なプロセスは、それ自身のステップ関数を用いて transducer を呼び出す際に生成する reducing 関数を expose すべきではありません。なぜなら、それは状態をもち、他の場所から使用することは安全でないからです。

== 一時性

#@# Transient

#@# Although ClojureScript's immutable and persistent data structures are reasonably performant there are situations in which we are transforming large data structures using multiple steps to only share the final result. For example, the core  into  function takes a collection and eagerly populates it with the contents of a sequence:
ClojureScriptの不変で永続的なデータ構造は、それなりのパフォーマンスを発揮しますが、最終的な結果を共有するだけのために、複数のステップを使って大きなデータ構造を変換している状況もあります。たとえば、core にある into 関数は、コレクションを取得して、シーケンスの内容を続々と取り込みます。

//emlist{
(into [] (range 100))
;; => [0 1 2 ... 98 99]
//}

#@# In the above example we are generating a vector of 100 elements  conj -ing one at a time. Every intermediate vector that is not the final result won't be seen by anybody except the  into  function and the array copying required for persistence is an unnecesary overhead.
上の例では、100 個の要素からなるベクタを一度に一つずつ @<code>{conj} しながら生成しています。最終的な結果でない全ての中間ベクタは into 関数以外では見ることができず、永続化のために必要な配列のコピーは不要なオーバーヘッドです。

#@# For these situations ClojureScript provides a special version of some of its persistent data structures, which are called transients. Maps, vectors and sets have a transient counterpart.  Transients are always derived from a persistent data structure using the  transient  function, which creates a transient version in constant time:
このような状況のために、ClojureScript は transient と呼ばれる永続的なデータ構造を提供しています。マップ、ベクタ、セットには、transient に相当するものがあります。Transient は、常に transient 関数を使用して、永続的なデータ構造から生成されます。これにより、一定の時間内に、transient のバージョンが作成されます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def tv (transient [1 2 3]))
;; => #<[object Object]>
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Transients support the read API of their persistent counterparts:
Transient は、対応する永続的な読取り API をサポートします。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def tv (transient [1 2 3]))

(nth tv 0)
;; => 1
//}


#@# Page120
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

//emlist{
(get tv 2)
;; => 3

(def tm (transient {:language "ClojureScript"}))

(:language tm)
;; => "ClojureScript"

(def ts (transient #{:a :b :c}))

(contains? ts :a)
;; => true

(:a ts)
;; => :a
//}

#@#***


#@# Since transients don't have persistent and immutable semantics for updates they can't be transformed using the already familiar  conj  or  assoc  functions. Instead, the transforming functions that work on transients end with a bang. Let's look at an example using  conj!  on a transient:
transient には更新のための永続的で不変なセマンティクスがないので、@<code>{conj} や @<code>{assoc} 関数を使って変換できません。その代わり、transient 上で機能する変換関数は感嘆符 ! をつけます。@<code>{conj!} を使った例を見てみましょう。

//emlist{
(def tv (transient [1 2 3]))

(conj! tv 4)
;; => #<[object Object]>

(nth tv 3)
;; => 4
//}

#@# As you can see, the transient version of the vector is neither immutable nor persistent. Instead, the vector is mutated in place. Although we could transform  tv  repeatedly using  conj!  on it we shouldn't abandon the idioms used with the persistent data structures: when transforming a transient, use the returned version of it for further modifications like in the following example:
このように、ベクタの transient バージョンは不変でも永続でもありません。その代わり、ベクタはその場で変異します。@<code>{conj!} を使って繰り返し @<code>{tv} を変換することもできますが、永続的なデータ構造で使用されるイディオムを放棄してはいけません。transient のデータを変換する場合は、次の例のように、その戻されたバージョンを使用して変更を加えます。

//emlist{
(-> [1 2 3]
  transient
  (conj! 4)
  (conj! 5))
;; => #<[object Object]>
//}

#@# Page121
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


#@# We can convert a transient back to a persistent and immutable data structure by calling  persistent!  on it. This operation, like deriving a transient from a persistent data structure, is done in constant time.
transient! を呼ぶことで、transient を元の永続的で不変なデータ構造に変換することもできます。この操作は、transient を永続的なデータ構造から派生させる操作と同様に、一定の時間で実行されます。

//emlist{
(-> [1 2 3]
  transient
  (conj! 4)
  (conj! 5)
  persistent!)
;; => [1 2 3 4 5]
//}

#@# A peculiarity of transforming transients into persistent structures is that the transient version is invalidated after being converted to a persistent data structure and we can't do further transformations to it. This happens because the derived persistent data structure uses the transient's internal nodes and mutating them would break the immutability and persistent guarantees:
transient を永続的な構造に変換する特徴は、一時的なものが永続的なデータ構造に変換された後に無効になり、それ以上の変換ができなくなることです。これは、派生した永続的なデータ構造が transient の内部的な node を使用しており、それらを変更することにより不変性と永続的な保証が壊れるためです。

//emlist{
(def tm (transient {}))
;; => #<[object Object]>

(assoc! tm :foo :bar)
;; => #<[object Object]>

(persistent! tm)
;; => {:foo :bar}

(assoc! tm :baz :frob)
;; Error: assoc! after persistent!
//}

#@# Going back to our initial example with  into , here's a very simplified implementation of it that uses a transient for performance, returning a persistent data structure and thus exposing a purely functional interface although it uses mutation internally:
最初の @<code>{into} の例に戻りましょう。ここではパフォーマンスのために transient を使用してシンプルに実装します。内部的には変異を使用しますが、永続的なデータ構造を返す純粋な関数型のインタフェースを公開します。

//emlist{
(defn my-into
  [to from]
  (persistent! (reduce conj! (transient to) from)))

(my-into [] (range 100))
;; => [0 1 2 ... 98 99]
//}


#@# Page122
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


== メタデータ

#@# Metadata

#@# ClojureScript symbols, vars and persistent collections support attaching metadata to them. Metadata is a map with information about the entity it's attached to. The ClojureScript compiler uses metadata for several purposes such as type hints, and the metadata system can be used by tooling, library and application developers too.
ClojureScriptのシンボル、変数、永続的なコレクションは、メタデータの添付(attach)をサポートしています。メタデータは、添付される実体(entity)に関する情報を含むマップです。ClojureScript のコンパイラは、型のヒントのような目的のためにメタデータを使用します。また、メタデータのシステムは、ツールやライブラリ及びアプリケーションの開発者も利用できます。

#@# There may not be many cases in day-to-day ClojureScript programming where you need metadata, but it is a nice language feature to have and know about; it may come in handy at some point. It makes things like runtime code introspection and documentation generation very easy. You'll see why throughout this section.
ClojureScript を日常的に書いているなかでは、メタデータが必要になることはあまりないかもしれませんが、メタデータは知っておくと便利な機能なので、いつか役に立つかもしれません。実行時のコード分析やドキュメントの生成などが非常に簡単になります。次のセクションでは、その理由を説明します。

=== Vars

#@# Let's define a var and see what metadata is attached to it by default. Note that this code is executed in a REPL, and thus the metadata of a var defined in a source file may vary. We'll use the  meta  function to retrieve the metadata of the given value:
var を定義して、デフォルトでどのメタデータが付加されているかを見てみましょう。このコードは REPL で実行されるため、ソースファイルで定義されている var のメタデータは変化するかもしれません。meta 関数を使用して、指定された値のメタデータを取得します。

//emlist{
(def answer-to-everything 42)
;; => 42

#'answer-to-everything
;; => #'cljs.user/answer-to-everyhing

(meta #'answer-to-everything)
;; => {:ns cljs.user,
;;     :name answer-to-everything,
;;     :file "NO_SOURCE_FILE",
;;     :source "answer-to-everything",
;;     :column 6,
;;     :end-column 26,
;;     :line 1,
;;     :end-line 1,
;;     :arglists (),
;;     :doc nil,
;;     :test nil}
//}

#@# Few things to note here. First of all,  #'answer-to-everything  gives us a reference to the  Var  that holds the value of the  answer-to-everything  symbol. We see that it includes information about the namespace ( :ns ) in which it was defined, its name, file (although, since it was defined at a REPL doesn't have a source file), source, position in the file where it was defined, argument list (which only makes sense for functions), documentation string and test function.
上記のコードにおいて、@<code>{#'answer-to-everything} は、@<code>{answer-to-everything} シンボルの値を保持する var への参照を渡します。これには、定義された名前空間@<code>{(:ns)}、その名前、ファイル(ここではREPLで定義されているため、ソースファイルはありません)、ソース、定義されたファイルでの位置、引数の一覧(関数の場合のみ意味があります)、ドキュメントの文字列、およびテスト関数に関する情報が含まれていることがわかります。

#@# Let's take a look at a function var's metadata:
関数における var のメタデータを見てみましょう。


#@# Page123
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(defn add
  "A function that adds two numbers."
  [x y]
  (+ x y))

(meta #'add)
;; => {:ns cljs.user,
;;     :name add,
;;     :file "NO_SOURCE_FILE",
;;     :source "add",
;;     :column 7,
;;     :end-column 10,
;;     :line 1,
;;     :end-line 1,
;;     :arglists (quote ([x y])),
;;     :doc "A function that adds two numbers.",
;;     :test nil}
//}

#@# We see that the argument lists are stored in the  :arglists  field of the var's metadata and its documentation in the  :doc  field. We'll now define a test function to learn about what  :test  is used for:
引数のリストは @<code>{:arglists} フィールド、マニュアルは @<code>{:doc} フィールドに格納されます。では テスト関数を定義して、何のために :test フィールドを使うかを見ていきましょう。

//emlist{
(require '[cljs.test :as t])

(t/deftest i-pass
  (t/is true))

(meta #'i-pass)
;; => {:ns cljs.user,
;;     :name i-pass,
;;     :file "NO_SOURCE_FILE",
;;     :source "i-pass",
;;     :column 12,
;;     :end-column 18,
;;     :line 1,
;;     :end-line 1,
;;     :arglists (),
;;     :doc "A function that adds two numbers.",
;;     :test #<function (){ ... }>}
//}

#@# The  :test  attribute (truncated for brevity) in the  i-pass  var's metadata is a test function. This is used by the  cljs.test  library for discovering and running tests in the namespaces you tell it to.
@<code>{i-pass} var のメタデータ内の @<code>{:test} フィールドがテスト関数です。テスト関数は、指定した名前空間でテストを検出して実行するために @<code>{cljs.test} ライブラリによって使用されます。


=== 値

#@# We learned that vars can have metadata and what kind of metadata is added to them for consumption by the compiler and the  cljs.test  testing library. Persistent collections can have metadata too, although they don't have any by default. We can use the  with-meta  function to derive an object with the same value and type with the given metadata attached. Let's see how:
var はメタデータを持つことができ、コンパイラと @<code>{cljs.test} ライブラリが利用するために、どのようなメタデータを付与されるかを学びました。永続コレクションがメタデータをもつことは可能ですが、デフォルトではメタデータはありません。@<code>{with-meta} 関数を使用すると、指定したメタデータが付加された同じ値と型を持つオブジェクトを派生させることができます。例を見てみましょう。


#@# Page124
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def map-without-metadata {:language "ClojureScript"})
;; => {:language "ClojureScript"}

(meta map-without-metadata)
;; => nil

(def map-with-metadata (with-meta map-without-metadata
                                  {:answer-to-everything 42}))
;; => {:language "ClojureScript"}

(meta map-with-metadata)
;; => {:answer-to-everything 42}

(= map-with-metadata
   map-without-metadata)
;; => true

(identical? map-with-metadata
            map-without-metadata)
;; => false
//}

#@# It shouldn't come as a surprise that metadata doesn't affect equality between two data structures since equality in ClojureScript is based on value. Another interesting thing is that  with-meta  creates another object of the same type and value as the given one and attaches the given metadata to it.
ClojureScript の等価性は値に基づいているため、メタデータが 2 つのデータ構造の等価性に影響しないことは驚くべきことではありません。もう 1 つ興味深い点は、with-meta を使用すると、指定されたものと同じ型と値をもつ別のオブジェクトが作成され、指定されたメタデータがそれに付与されることです。

#@# Another open question is what happens with metadata when deriving new values from a persistent data structure.  Let's find out:
永続データ構造から新しい値を派生させるときにメタデータで何が起こるかを確認してみましょう。

//emlist{
(def derived-map (assoc map-with-metadata :language "Clojure"))
;; => {:language "Clojure"}

(meta derived-map)
;; => {:answer-to-everything 42}
//}

#@# As you can see in the example above, metadata is preserved in derived versions of persistent data structures. There are some subtleties, though. As long as the functions that derive new data structures return collections with the same type, metadata will be preserved; this is not true if the types change due to the transformation. To ilustrate this point, let's see what happens when we derive a seq or a subvector from a vector:
この例でわかるように、メタデータは永続的なデータ構造の派生バージョンで保持されます。微妙なところもありますが、新しいデータ構造を派生する関数が同じ型のコレクションを返す限り、メタデータは保存されます。これは、変換によって型が変更される場合には当てはまりません。この点を理解するために、ベクタから seq や subvector を派生させると何が起こるかを見てみましょう。


#@# Page125
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def v (with-meta [0 1 2 3] {:foo :bar}))
;; => [0 1 2 3]

(def sv (subvec v 0 2))
;; => [0 1]

(meta sv)
;; => nil

(meta (seq v))
;; => nil
//}


=== メタデータのための構文

#@# Syntax for metadata

#@# The ClojureScript reader has syntactic support for metadata annotations, which can be written in different ways. We can prepend var definitions or collections with a caret ( ^ ) followed by a map for annotating it with the given metadata map:
ClojureScript の reader はメタデータの注釈(annotation)を構文としてサポートしますが、様々な方法で記述できます。var 定義やコレクションの先頭にキャレット(^)とその後にマップをつけて、指定したメタデータのマップで注釈をつけることができます。

//emlist{
(def ^{:doc "The answer to Life, Universe and Everything."} answer-to-everything 42)
;; => 42

(meta #'answer-to-everything)
;; => {:ns cljs.user,
;;     :name answer-to-everything,
;;     :file "NO_SOURCE_FILE",
;;     :source "answer-to-everything",
;;     :column 6,
;;     :end-column 26,
;;     :line 1,
;;     :end-line 1,
;;     :arglists (),
;;     :doc "The answer to Life, Universe and Everything.",
;;     :test nil}

(def map-with-metadata ^{:answer-to-everything 42} {:language "ClojureScript"})
;; => {:language "ClojureScript"}

(meta map-with-metadata)
;; => {:answer-to-everything 42}
//}

#@# Notice how the metadata given in the  answer-to-everything  var definition is merged with the var metadata.
@<code>{answer-to-everything} の var の定義で指定されたメタデータが var のメタデータとどのようにマージされるかに注意してください。

#@# A very common use of metadata is to set certain keys to a  true  value. For example we may want to add to a var's metadata that the variable is dynamic or a constant. For such cases, we have a shorthand notation that uses a caret followed by a keyword. Here are some examples:
メタデータの一般的な使用法は、特定のキーの値を @<code>{true} に設定することです。たとえば、変数が動的まか定数かを var のメタデータに追加できます。このような場合には、キャレットに続けてキーワードを使用する省略表記があります。次に例を示します。


#@# Page126
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def ^:dynamic *foo* 42)
;; => 42

(:dynamic (meta #'*foo*))
;; => true

(def ^:foo ^:bar answer 42)
;; => 42

(select-keys (meta #'answer) [:foo :bar])
;; => {:foo true, :bar true}
//}

#@# There is another shorthand notation for attaching metadata. If we use a caret followed by a symbol it will be added to the metadata map under the  :tag  key. Using tags such as  ^boolean  gives the ClojureScript compiler hints about the type of expressions or function return types.
メタデータの付与には、別の簡略表記があります。キャレットに続けてシンボルを使用すると、:tag キーの下のメタデータのマップに追加されます。^boolean などのタグを使用すると、ClojureScript コンパイラに、式の型や関数の戻り値の型に関するヒントを与えます。

//emlist{
(defn ^boolean will-it-blend? [_] true)
;; => #<function ... >

(:tag (meta #'will-it-blend?))
;; => boolean

(not ^boolean (js/isNaN js/NaN))
;; => false
//}

=== メタデータを扱う関数

#@# Functions for working with metadata

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# We've learned about  meta  and  with-meta  so far but ClojureScript offers a few functions for transforming metadata. There is  vary-meta  which is similar to  with-meta  in that it derives a new object with the same type and value as the original but it doesn't take the metadata to attach directly. Instead, it takes a function to apply to the metadata of the given object to transform it for deriving new metadata. This is how it works:
これまで @<code>{meta} と @<code>{with-meta} について学んできましたが、ClojureScript にはメタデータを変換するための関数がいくつか用意されています。@<code>{var-meta} は、元のオブジェクトと同じ型と値を持つ新しいオブジェクトを派生させるという点で @<code>{with-meta} と似ていますが、直接的に付与するメタデータをとりません。代わりに、var-meta は、変換する指定のオブジェクトのメタデータに適用する関数をとり、新たなメタデータを派生させます。例を見てみましょう。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def map-with-metadata ^{:foo 40} {:language "ClojureScript"})
;; => {:language "ClojureScript"}

(meta map-with-metadata)
;; => {:foo 40}

(def derived-map (vary-meta map-with-metadata update :foo + 2))
;; => {:language "ClojureScript"}

(meta derived-map)
;; => {:foo 42}
//}


#@# Page127
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# If instead we want to change the metadata of an existing var or value we can use  alter-meta!  for changing it by applying a function or  reset-meta!  for replacing it with another map:
既存の var や値のメタデータを変更する場合、変更のためには @<code>{alter-meta!} 関数を、別のマップに置き換えるには @<code>{reset-meta!} を使うことができます。

//emlist{
(def map-with-metadata ^{:foo 40} {:language "ClojureScript"})
;; => {:language "ClojureScript"}

(meta map-with-metadata)
;; => {:foo 40}

(alter-meta! map-with-metadata update :foo + 2)
;; => {:foo 42}

(meta map-with-metadata)
;; => {:foo 42}

(reset-meta! map-with-metadata {:foo 40})
;; => {:foo 40}

(meta map-with-metadata)
;; => {:foo 40}
//}


#@# TODO: metadata on functions and multiarity functions


#@# Page128
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


== 中心的なプロトコル

#@# Core protocols

#@# One of the greatest qualities of the core ClojureScript functions is that they are implemented around protocols. This makes them open to work on any type that we extend with such protocols, be it defined by us or a third party.
ClojureScript の core に定義されている関数群がプロトコルを中心に実装されていることも素晴らしい特徴の 1 つだといえます。これは、そのようなプロトコルで拡張したどんな型、また私たちや第三者によって定義された型でも動作するように開かれています。


=== 関数

#@# Functions

#@# As we've learned in previous chapters not only functions can be invoked. Vectors are functions of their indexes, maps are functions of their keys and sets are functions of their values.
これまでの章で学んだように、関数だけが呼び出されるものとは限りません。ベクタはインデックス、マップはキー、セットは値を関数のように使うことができます。

#@# We can extend types to be callable as functions implementing the  IFn  protocol. A collection that doesn't support calling it as a function is the queue, let's implement  IFn  for the  PersistentQueue  type so we're able to call queues as functions of their indexes:
@<code>{IFn} プロトコルを実装する関数として呼び出し可能な型に拡張することができます。関数としての呼び出しをサポートしていないコレクションが queue です。@<code>{PersistentQueue} 型に @<code>{IFn} を実装して、queue をインデックスの関数として呼び出すことができるようにします。


@<embed>{|latex|\vspace{-0.3\Cvs\}}


//emlist{
(extend-type PersistentQueue
  IFn
  (-invoke
    ([this idx]
      (nth this idx))))

(def q #queue[:a :b :c])
;; => #queue [:a :b :c]

(q 0)
;; => :a

(q 1)
;; => :b

(q 2)
;; => :c
//}


=== Printing

#@# Printing

#@# TODO: IWriter?

#@# For learning about some of the core protocols we'll define a  Pair  type, which will hold a pair of values.
いくつかの中心的なプロトコルについて学ぶために、値のペアを保持する @<code>{Pair} 型を定義します。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(deftype Pair [fst snd])
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@#***

#@# If we want to customize how types are printed we can implement the  IPrintWithWriter  protocol. It defines a function called  -pr-writer  that receives the value to print, a writer object and options; this function uses the writer object's  -write  function to write the desired  Pair  string representation:
型の印字方法をカスタマイズしたい場合は、IPrintWithWriter プロトコルを実装できます。出力値、writer オブジェクト、オプションを受け取る @<code>{-pr-writer} と呼ばれる関数を定義します。この関数は、求められる @<code>{Pair}型 の文字列表現を書くために、writer オブジェクトの @<code>{-write} 関数を使用します。

#@# Page129
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-type Pair
  IPrintWithWriter
  (-pr-writer [p writer _]
    (-write writer (str "#<Pair " (.-fst p) "," (.-snd p) ">"))))
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

=== シーケンス

#@# Sequences

#@# In a xref:the-sequence-abstraction[previous section] we learned about sequences, one of ClojureScript's main abstractions. Remember the  first  and  rest  functions for working with sequences? They are defined in the  ISeq  protocol, so we can extend types for responding to such functions:
前のセクションでは、ClojureScript の主要な抽象概念の 1 つであるシーケンスについて学びました。シーケンスを操作するための @<code>{first} 関数と @<code>{rest} 関数を覚えているでしょうか。これらは @<code>{ISeq} プロトコルで定義されているので、このような関数に対応する型を拡張することができます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(extend-type Pair
  ISeq
  (-first [p]
    (.-fst p))

  (-rest [p]
    (list (.-snd p))))

(def p (Pair. 1 2))
;; => #<Pair 1,2>

(first p)
;; => 1

(rest p)
;; => (2)
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Another handy function for working with sequences is  next . Although  next  works as long as the given argument is a sequence, we can implement it explicitly with the  INext  protocol:
シーケンスを操作する便利な関数として @<code>{next} があります。@<code>{next} は、与えられた引数がシーケンスである限り動作しますが、@<code>{INext} プロトコルを使って明示的に実装することができます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def p (Pair. 1 2))

(next p)
;; => (2)

(extend-type Pair
  INext
  (-next [p]
    (println "Our next")
    (list (.-snd p))))

(next p)
;; Our next
;; => (2)
//}


#@# Page130
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# Finally, we can make our own types seqable implementing the  ISeqable  protocol. This means we can pass them to  seq  for getting a sequence back.
最後に、@<code>{ISeqable} プロトコルを実装する独自の型をシーカブルにすることができます。これは、シーケンスを元に戻すために @<code>{seq} に渡すことができることを意味します。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# ISeqable

//emlist{
(def p (Pair. 1 2))

(extend-type Pair
  ISeqable
  (-seq [p]
    (list (.-fst p) (.-snd p))))

(seq p)
;; => (1 2)
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Now our  Pair  type works with the plethora of ClojureScript functions for working with sequences:
これで @<code>{Pair} 型は、ClojureScript のシーケンス操作関数の大半で動作します。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(def p (Pair. 1 2))
;; => #<Pair 1,2>

(map inc p)
;; => (2 3)

(filter odd? p)
;; => (1)

(reduce + p)
;; => 3
//}


=== コレクション

#@# Collections

#@# Collection functions are also defined in terms of protocols. For this section examples we will make the native JavaScript string act like a collection.
#@# The most important function for working with collection is  conj , defined in the  ICollection  protocol.  Strings are the only type which makes sense to  conj  to a string, so the  conj  operation for strings will be simply a concatenation:
コレクションの関数もプロトコルで定義されています。このセクションの例では、native な JavaScript の文字列をコレクションのように動作させます。
コレクションを扱う上で最も重要な関数は、@<code>{ICollection} プロトコルで定義されている @<code>{conj} です。文字列は、文字列への @<code>{conj} に意味を持つ唯一の型であるため、文字列に対する @<code>{conj} の操作は単なる連結になります。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(extend-type string
  ICollection
  (-conj [this o]
    (str this o)))

(conj "foo" "bar")
;; => "foobar"

(conj "foo" "bar" "baz")
;; => "foobarbaz"
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Page131
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# Another handy function for working with collections is  empty , which is part of the  IEmptyable Collection  protocol. Let's implement it for the string type:
コレクションを操作する @<code>{empty} は便利な関数です。これは @<code>{IEmptyable} Collection プロトコルの一部です。文字列の型に実装してみましょう。

//emlist{
(extend-type string
  IEmptyableCollection
  (-empty [_]
    ""))

(empty "foo")
;; => ""
//}

#@# We used the  string  special symbol for extending the native JavaScript string. If you want to learn more about it check the xref:extending-javascript-types[section about extending JavaScript types].
ここでは、native な JavaScript の文字列を拡張するための特別なシンボルである @<code>{string} を使いました。詳細については、JavaScript の型を拡張するセクションを参照してください。


===== コレクションの特性

#@# Collection traits

#@# There are some qualities that not all collections have, such as being countable in constant time or being reversible. These traits are splitted into different protocols since not all of them make sense for every collection. For illustrating these protocols we'll use the  Pair  type we defined earlier.
すべてのコレクションに備わっているわけではない特性がいくつかあります。たとえば、一定の時間で数えられることや、可逆的であることなどです。これらの特性がすべてのコレクションに対して意味をもつわけではないので、これらの特性は異なるプロトコルに分割されます。これらのプロトコルを説明するために、前に定義した Pair 型を使用します。

#@# For collections that can be counted in constant time using the  count  function we can implement the  ICounted  protocol. It should be easy to implement it for the  Pair  type:
@<code>{count} 関数を使用して一定の時間でカウントできるコレクションの場合、@<code>{ICounted} プロトコルを実装できます。@<code>{Pair}型には容易に実装できるはずです。

//emlist{
(extend-type Pair
  ICounted
  (-count [_]
    2))

(def p (Pair. 1 2))

(count p)
;; => 2
//}

#@# Some collection types such as vectors and lists can be indexed by a number using the  nth  function. If our types are indexed we can implement the  IIndexed  protocol:
ベクタやリストなどのコレクション型は @<code>{nth} 関数でインデックス番号を付けることができます。このような型にインデックスが付けられれば、@<code>{IIndexed} プロトコルを実装することができます。

#@# Page132
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-type Pair
  IIndexed
  (-nth
    ([p idx]
     (case idx
       0 (.-fst p)
       1 (.-snd p)
       (throw (js/Error. "Index out of bounds"))))
    ([p idx default]
     (case idx
       0 (.-fst p)
       1 (.-snd p)
       default))))

(nth p 0)
;; => 1

(nth p 1)
;; => 2

(nth p 2)
;; Error: Index out of bounds

(nth p 2 :default)
;; => :default
//}

#@# TODO: ISorted?

#@# TODO: IReversible?

=== 連想性

#@# Associative

#@# There are many data structures that are associative: they map keys to values. We've encountered a few of them already and we know many functions for working with them like  get ,  assoc  or  dissoc . Let's explore the protocols that these functions build upon.
キーを値にマップする連想的なデータ構造が数多くあります。私たちはすでにいくつかの関数に遭遇しており、get、assoc、dissoc 等の関数を知っています。これらの関数を構築しているプロトコルについて説明します。

#@# First of all, we need a way to look up keys on an associative data structure. The  ILookup  protocol defines a function for doing so, let's add the ability to look up keys in our  Pair  type since it is an associative data structure that maps the indices 0 and 1 to values.
まず最初に、連想的なデータ構造においてキーを検索する方法が必要です。@<code>{ILookup} プロトコルは、そのための関数を定義します。ここでは、Pair 型にキーを検索する機能を追加します。これは、Pair 型がインデックス 0 と 1 を値にマップする連想的なデータ構造であるためです。

//emlist{
(extend-type Pair
  ILookup
  (-lookup
    ([p k]
     (-lookup p k nil))
    ([p k default]
     (case k
       0 (.-fst p)
       1 (.-snd p)
       default))))
//}


#@# Page133
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(get p 0)
;; => 1

(get p 1)
;; => 2

(get p :foo)
;; => nil

(get p 2 :default)
;; => :default
//}

#@# For using  assoc  on a data structure it must implement the  IAssociative  protocol. For our  Pair  type only 0 and 1 will be allowed as keys for associating values.  IAssociative  also has a function for asking whether a key is present or not.
データ構造で @<code>{assoc} を使用するには、@<code>{IAssociative} プロトコルを実装する必要があります。@<code>{Pair} 型では、値を関連づけるためのキーとして 0 と 1 だけが許可されます。@<code>{IAssociative} には、キーの有無を問い合わせる機能もあります。

//emlist{
(extend-type Pair
  IAssociative
  (-contains-key? [_ k]
    (contains? #{0 1} k))

  (-assoc [p k v]
    (case k
      0 (Pair. v (.-snd p))
      1 (Pair. (.-fst p) v)
      (throw (js/Error. "Can only assoc to 0 and 1 keys")))))

(def p (Pair. 1 2))
;; => #<Pair 1,2>

(assoc p 0 2)
;; => #<Pair 2,2>

(assoc p 1 1)
;; => #<Pair 1,1>

(assoc p 0 0 1 1)
;; => #<Pair 0,1>

(assoc p 2 3)
;; Error: Can only assoc to 0 and 1 keys
//}

#@# The complementary function for  assoc  is  dissoc  and it's part of the  IMap  protocol. It doesn't make much sense for our  Pair  type but we'll implement it nonetheless. Dissociating 0 or 1 will mean putting a  nil  in such position and invalid keys will be ignored.
@<code>{assoc} を補完する関数として @<code>{dissoc} があり、@<code>{dissoc} は @<code>{IMap}プロトコルの一部です。@<code>{Pair} 型ではあまり意味がありませんが、それでも実装してみましょう。0 または 1 の関連付けを解除すると、そのような位置に @<code>{nil} が配置され、無効なキーは無視されます。


#@# Page134
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-type Pair
  IMap
  (-dissoc [p k]
    (case k
      0 (Pair. nil (.-snd p))
      1 (Pair. (.-fst p) nil)
      p)))

(def p (Pair. 1 2))
;; => #<Pair 1,2>

(dissoc p 0)
;; => (nil 2)

(dissoc p 1)
;; => (1 nil)

(dissoc p 2)
;; => (1 2)

(dissoc p 0 1)
;; => (nil nil)
//}

#@# Associative data structures are made of key and value pairs we can call entries. The  key  and  val  functions allow us to query the key and value of such entries and they are built upon the  IMapEntry  protocol. Let's see a few examples of  key  and  val  and how map entries can be used to build up maps:
連想的なデータ構造は、エントリー(entries)と呼ばれるキーと値のペアで構成されます。@<code>{key} 関数と @<code>{val} 関数を使用すると、このようなエントリーのキーと値を参照できます。これらの関数は @<code>{IMapEntry} プロトコルに基づいて構築されています。@<code>{key} 関数と @<code>{val} 関数の例と、マップのエントリーを使用してマップを構築する方法をいくつか見てみましょう。

//emlist{
(key [:foo :bar])
;; => :foo

(val [:foo :bar])
;; => :bar

(into {} [[:foo :bar] [:baz :xyz]])
;; => {:foo :bar, :baz :xyz}
//}

#@# Pairs can be map entries too, we treat their first element as the key and the second as the value:
Pair 型もマップのエントリーにできます。最初の要素をキー、2 番目の要素を値として扱います。

//emlist{
(extend-type Pair
  IMapEntry
  (-key [p]
    (.-fst p))

  (-val [p]
    (.-snd p)))
//}


#@# Page135
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def p (Pair. 1 2))
;; => #<Pair 1,2>

(key p)
;; => 1

(val p)
;; => 2

(into {} [p])
;; => {1 2}
//}


=== 比較

#@# Comparison

#@# For checking whether two or more values are equivalent with  =  we must implement the  IEquiv  protocol. Let's do it for our  Pair  type:
2 つ以上の値が等しいかどうかを @<code>{=} を用いて判定するには、@<code>{IEquiv}プロトコルを実装する必要があります。@<code>{Pair} 型に、@<code>{IEquiv} プロトコルを実装してみましょう:

//emlist{
(def p  (Pair. 1 2))
(def p' (Pair. 1 2))
(def p'' (Pair. 1 2))

(= p p')
;; => false

(= p p' p'')
;; => false

(extend-type Pair
  IEquiv
  (-equiv [p other]
    (and (instance? Pair other)
         (= (.-fst p) (.-fst other))
         (= (.-snd p) (.-snd other)))))

(= p p')
;; => true

(= p p' p'')
;; => true
//}

#@# TODO: IHash?

#@# We can also make types comparable. The function  compare  takes two values and returns a negative number if the first is less than the second, 0 if both are equal and 1 if the first is greater than the second. For making our types comparable we must implement the  IComparable  protocol.
型を比較できるようにすることもできます。@<code>{compare} 関数は 2 つの値を取り、最初の値が 2 番目の値より小さい場合は負の数を返し、両方が等しい場合は 0 を返し、最初の値が 2 番目の値より大きい場合は 1 を返します。型を比較できるようにするには、@<code>{IComparable} プロトコルを実装する必要があります。

#@# For pairs, comparison will mean checking if the two first values are equal. If they are, the result will be the comparison of the second values. If not, we will return the result of the first comparison:
Pair 型の場合、比較とは最初の 2 つの値が等しいかどうかをチェックすることを意味します。等しい場合は、結果は 2 番目の値になります。そうでない場合は、最初の比較の結果を返します。


#@# Page136
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-type Pair
  IComparable
  (-compare [p other]
    (let [fc (compare (.-fst p) (.-fst other))]
      (if (zero? fc)
        (compare (.-snd p) (.-snd other))
        fc))))

(compare (Pair. 0 1) (Pair. 0 1))
;; => 0

(compare (Pair. 0 1) (Pair. 0 2))
;; => -1

(compare (Pair. 1 1) (Pair. 0 2))
;; => 1

(sort [(Pair. 1 1) (Pair. 0 2) (Pair. 0 1)])
;; => ((0 1) (0 2) (1 1))
//}


=== メタデータ

#@# Metadata

#@# The  meta  and  with-meta  functions are also based upon two protocols:  IMeta  and  IWithMeta  respectively.  We can make our own types capable of carrying metadata adding an extra field for holding the metadata and implementing both protocols.
@<code>{meta} 関数と @<code>{with-meta} 関数も、それぞれ @<code>{IMeta} と @<code>{IWithMeta} という 2 つのプロトコルに基づいています。追加のフィールドをメタデータを保持するために加えて、両プロトコルを実装することにより、メタデータを運ぶ独自の型を作成できます。

#@# Let's implement a version of  Pair  that can have metadata:
メタデータをもつことができる @<code>{Pair} を実装して見ましょう。

//emlist{
(deftype Pair [fst snd meta]
  IMeta
  (-meta [p] meta)

  IWithMeta
  (-with-meta [p new-meta]
    (Pair. fst snd new-meta)))


(def p (Pair. 1 2 {:foo :bar}))
;; => #<Pair 1,2>

(meta p)
;; => {:foo :bar}

(def p' (with-meta p {:bar :baz}))
;; => #<Pair 1,2>

(meta p')
;; => {:bar :baz}
//}


#@# Page137
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{0mm}
//}
//embed[latex]{
\clearpage
//}


=== JavaScript との相互運用

#@# Interoperability with JavaScript

#@# Since ClojureScript is hosted in a JavaScript VM we often need to convert ClojureScript data structures to JavaScript ones and viceversa. We also may want to make native JS types participate in an abstraction represented by a protocol.
ClojureScript は JavaScript の VM でホストされるため、ClojureScript のデータ構造を JavaScript のデータ構造に変換したり、また、その逆を行う必要があります。native の JavaScript の型をプロトコルで表現される抽象化に加えることもできます。

===== JavaScript の型を拡張する

#@# Extending JavaScript types

#@# When extending JavaScript objects instead of using JS globals like  js/String ,  js/Date  and such, special symbols are used. This is done for avoiding mutating global JS objects.
JavaScript のオブジェクトを拡張する場合は、@<code>{js/String} や @<code>{js/Date} などの JavaScript のグローバル環境を使用せずに、特別なシンボルが使用されます。これは、グローバル環境にある JavaScript オブジェクトの変更を避けるために行われます。

#@# The symbols for extending JS types are:  object ,  array ,  number ,  string ,  function ,  boolean  and  nil  is used for the null object. The dispatch of the protocol to native objects uses Google Closure's link:https://google.github.io/closure-library/api/namespace_goog.html#typeOf[goog.typeOf] function. There's a special  default  symbol that can be used for making a default implementation of a protocol for every type.
JavaScript の型を拡張するシンボルには、@<code>{object}、@<code>{array}、@<code>{number}、@<code>{string}、@<code>{function}、@<code>{boolean}、@<code>{nil} があります。プロトコルを native オブジェクトにディスパッチするには、Google Closure の goog.typeOf 関数を使います。全ての型のためのプロトコルを実装するために使われる特別なデフォルトのシンボルがあります。

#@# For illustrating the extension of JS types we are going to define a  MaybeMutable  protocol that'll have a  mutable?  predicate as its only function. Since in JavaScript mutability is the default we'll extend the default JS type returning true from  mutable? :
JavaScript の型の拡張を説明するために、@<code>{mutable?} 関数を用いて @<code>{MaybeMutable} プロトコルを実装します。JavaScript の可変性はデフォルトなので、@<code>{mutable?} から true を返すデフォルトの JavaScript の型を拡張します。:

//emlist{
(defprotocol MaybeMutable
  (mutable? [this]
    "Returns true if the value is mutable."))

(extend-type default
  MaybeMutable
  (mutable? [_] true))

;; object
(mutable? #js {})
;; => true

;; array
(mutable? #js [])
;; => true

;; string
(mutable? "")
;; => true

;; function
(mutable? (fn [x] x))
;; => true
//}

#@# Since fortunately not all JS object's values are mutable we can refine the implementation of  MaybeMutable  for returning  false  for strings and functions.
幸い、全ての JavaScript オブジェクトの値が可変という訳ではないため、文字列と関数に対して @<code>{false} を返すように @<code>{MaybeMutable} の実装を改良することができます。


#@# Page138
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-protocol MaybeMutable
  string
  (mutable? [_] false)

  function
  (mutable? [_] false))

;; object
(mutable? #js {})
;; => true

;; array
(mutable? #js [])
;; => true

;; string
(mutable? "")
;; => false

;; function
(mutable? (fn [x] x))
;; => false
//}

#@# There is no special symbol for JavaScript dates so we have to extend  js/Date  directly. The same applies to the rest of the types found in the global  js  namespace.
JavaScript では date に特別な記号はないので、@<code>{js/Date} を直接拡張する必要があります。JavaScript のグローバルな名前空間にある残りの型にも同じことが当てはまります。

===== データの変換

#@# Converting data

#@# For converting values from ClojureScript types to JavaScript ones and viceversa we use the  clj->js  and  js->clj  functions, which are based in the  IEncodeJS  and  IEncodeClojure  protocols respectively.
ClojureScript の型から JavaScript の型、あるいはその逆を行うために、@<code>{IEncodeJS} と @<code>{IEncodeClojure} プロトコルにそれぞれ基づいた @<code>{clj->js} と @<code>{js->clj} 関数を使います。

#@# For the examples we'll use the Set type introduced in ES6. Note that is not available in every JS runtime.
この例では、ES6 で導入された Set 型を使用します。これは、全ての JavaScript の実行環境で使用できるわけではありません。

===== ClojureScript から JavaScript

#@# From ClojureScript to JS

#@# First of all we'll extend ClojureScript's set type for being able to convert it to JS. By default sets are converted to JavaScript arrays:
まず、ClojureScript の set 型を拡張して JavaScript に変換できるようにします。デフォルトでは、set は JavaScript の配列に変換されます。

//emlist{
(clj->js #{1 2 3})
;; => #js [1 3 2]
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# Let's fix it,  clj->js  is supposed to convert values recursively so we'll make sure to convert all the set contents to JS and creating the set with the converted values:
これを修正しましょう。@<code>{clj->js} は値を再帰的に変換するので、set の要素を全て JavaScript に変換して、変換された値を使って set を作成します。

@<embed>{|latex|\vspace{-0.3\Cvs\}}

//emlist{
(extend-type PersistentHashSet
  IEncodeJS
  (-clj->js [s]
    (js/Set. (into-array (map clj->js s)))))
//}


#@# Page139
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def s (clj->js #{1 2 3}))
(es6-iterator-seq (.values s))
;; => (1 3 2)

(instance? js/Set s)
;; => true

(.has s 1)
;; => true
(.has s 2)
;; => true
(.has s 3)
;; => true
(.has s 4)
;; => false
//}

#@# The  es6-iterator-seq  is an experimental function in ClojureScript core for obtaining a seq from an ES6 iterable.
@<code>{es6-iterator-seq} は、ES6 のイテレブルから seq を取得するための ClojureScript core にある実験的な関数です。

===== JavaScript から ClojureScript

#@# From JS to ClojureScript


#@# Now it's time to extend the JS set to convert to ClojureScript. As with  clj->js ,  js->clj  recursively converts the value of the data structure:
今度は JavaScript のセットを拡張して ClojureScript に変換します。@<code>{clj->js} と同様に、@<code>{js->clj} はデータ構造の値を再帰的に変換します。

//emlist{
(extend-type js/Set
  IEncodeClojure
  (-js->clj [s options]
    (into #{} (map js->clj (es6-iterator-seq (.values s))))))

(= #{1 2 3}
   (js->clj (clj->js #{1 2 3})))
;; => true

(= #{[1 2 3] [4 5] [6]}
   (js->clj (clj->js #{[1 2 3] [4 5] [6]})))
;; => true
//}

#@# Note that there is no one-to-one mapping between ClojureScript and JavaScript values. For example, ClojureScript keywords are converted to JavaScript strings when passed to  clj->js .
ClojureScript と JavaScript の値は、一対一のマッピングはないことに注意してください。たとえば、ClojureScript のキーワードは、@<code>{clj->js} に渡されると JavaScript の文字列に変換されます。


=== リダクション

#@# Reductions

#@# The  reduce  function is based on the  IReduce  protocol, which enables us to make our own or third-party types reducible. Apart from using them with  reduce  they will automatically work with  transduce  too, which will allow us to make a reduction with a transducer.
@<code>{reduce} 関数は @<code>{IReduce} プロトコルに基づいており、このプロトコルを使用すると、独自の型やサードパーティの型を reducible にすることができます。@<code>{reduce} と一緒に使う以外にも、自動的に @<code>{transduce} と一緒に動作するので、transducer  を使って reduce を行うことができます。

#@# The JS array is already reducible in ClojureScript:
JavaScript の配列は ClojureScript でも reduce できます。


#@# Page140
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(reduce + #js [1 2 3])
;; => 6

(transduce (map inc) conj [] [1 2 3])
;; => [2 3 4]
//}

#@# However, the new ES6 Set type isn't so let's implement the  IReduce  protocol. We'll get an iterator using the Set's  values  method and convert it to a seq with the  es6-iterator-seq  function; after that we'll delegate to the original  reduce  function to reduce the obtained sequence.

しかし、新しい ES6 に Set 型はそうではないので、@<code>{IReduce} プロトコルを実装しましょう。Set の @<code>{values} メソッドを使ってイテレーターを取得して、これを @<code>{es6-iterator-seq} 関数を使って seq に変換します。その後、元の @<code>{reduce} 関数にデリゲートして、取得したシーケンスを reduce します。

//emlist{
(extend-type js/Set
  IReduce
  (-reduce
    ([s f]
     (let [it (.values s)]
       (reduce f (es6-iterator-seq it))))
    ([s f init]
     (let [it (.values s)]
       (reduce f init (es6-iterator-seq it))))))

(reduce + (js/Set. #js [1 2 3]))
;; => 6

(transduce (map inc) conj [] (js/Set. #js [1 2 3]))
;; => [2 3 4]
//}

#@# Associative data structures can be reduced with the  reduce-kv  function, which is based in the  IKVReduce  protocol. The main difference between  reduce  and  reduce-kv  is that the latter uses a three-argument function as a reducer, receiving the accumulator, key and value for each item.
連想的なデータ構造は @<code>{IKVReduce} プロトコルに基づく @<code>{reduce‐kv} 関数を用いて reduce できます。@<code>{reduce} と @<code>{reduce-kv} の主な違いは、@<code>{reduce-kv} は 3 つの引数をとる関数であり、reducer、アキュムレータの受け取り、各アイテムのキーと値をとることです。

#@# Let's look at an example, we will reduce a map to a vector of pairs. Note that, since vectors associate indexes to values, they can also be reduced with  reduce-kv .
例を見てみましょう。マップをペアからなるベクタに変換します。ベクタはインデックスを値に関連付けるので、@<code>{reduce-kv} を使って値を reduce することができます。

//emlist{
(reduce-kv (fn [acc k v]
             (conj acc [k v]))
           []
           {:foo :bar
            :baz :xyz})
;; => [[:foo :bar] [:baz :xyz]]
//}

#@# We'll extend the new ES6 map type to support  reduce-kv , we'll do this by getting a sequence of key-value pairs and calling the reducing function with the accumulator, key and value as positional arguments:
新しい ES6 のマップ型を拡張して、@<code>{reduce-kv} をサポートするようにします。これを行うには、キーと値のペアのシーケンスを取得し、アキュムレータ、キー、値を位置を示す引数として使用して reducing 関数を呼び出します。


#@# Page141
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{10mm}
//}
//embed[latex]{
\clearpage
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//emlist{
(extend-type js/Map
  IKVReduce
  (-kv-reduce [m f init]
    (let [it (.entries m)]
      (reduce (fn [acc [k v]]
                (f acc k v))
              init
              (es6-iterator-seq it)))))

(def m (js/Map.))
(.set m "foo" "bar")
(.set m "baz" "xyz")

(reduce-kv (fn [acc k v]
             (conj acc [k v]))
           []
           m)
;; => [["foo" "bar"] ["baz" "xyz"]]
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# In both examples we ended up delegating to the  reduce  function, which is aware of reduced values and terminates when encountering one. Take into account that if you don't implement these protocols in terms of  reduce  you will have to check for reduced values for early termination.
どちらの例でも、最終的には @<code>{reduce} 関数にデリゲートすることになりました。reduce 関数は、値の reduce された値を認識して、値が見つかると終了します。これらのプロトコルを @<code>{reduce} に関して実装しない場合は、早期終了のために reduce された値をチェックする必要があることを覚えておいてください。

=== 遅延計算

#@# Delayed computation

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# There are some types that have the notion of asynchronous computation, the value they represent may not be realized yet. We can ask whether a value is realized using the  realized?  predicate.
#@# Let's ilustrate it with the  Delay  type, which takes a computation and executes it when the result is needed.  When we dereference a delay the computation is run and the delay is realized:
非同期処理の概念をもついくつかの型があり、それらが表す値はまだ認識(realize)されていないかもしれません。@<code>{realized?} を使って値が認識されたかどうかを確認することができます。
ここでは、計算を行い、結果が必要になったときにそれを実行する @<code>{Delay} 型を使って見ていきましょう。遅延を逆に参照すると、計算が実行されて、遅延が認識されます。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//emlist{
(defn computation []
  (println "running!")
  42)

(def d (delay (computation)))

(realized? d)　　;; => false

(deref d)
;; running!
;; => 42

(realized? d)　　;; => true

@d　　;; => 42
//}

@<embed>{|latex|\vspace{-0.5\Cvs\}}

#@# Both  realized?  and  deref  sit atop two protocols:  IPending  and  IDeref .
@<code>{realized?} と @<code>{deref} は @<code>{IPending} と @<code>{IDeref} のプロトコルの上にあります。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page142
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


=== 状態

#@# State

#@# The ClojureScript state constructs such as the Atom and the Volatile have different characteristics and semantics, and the operations on them like  add-watch ,  reset!  or  swap!  are backed by protocols.
Atom や Volatile のような ClojureScript の状態に関する構造体(state constructs)は、異なる特性とセマンティクスを持ち、add-watch や reset! または swap! のような操作は、プロトコルによってサポートされています。

===== Atom

#@# For ilustrating such protocols we will implement our own simplified version of an  Atom . It won't support validators nor metadata, but we will be able to:
このようなプロトコルを理解するために、私たちは独自の簡素版の @<code>{Atom} を実装します。バリデータやメタデータはサポートしませんが、次のようなことは可能です。

#@# -  deref  the atom for getting its current value
#@# -  reset!  the value contained in the atom
#@# -  swap!  the atom with a function for transforming its state

@<embed>{|latex|\vspace{0.5\Cvs\}}

- deref で現在の値を取得するためのアトムの参照解除する

- reset でアトムに含まれる値をリセットする

- swap! でatom を状態を変える関数を用いて変更する

@<embed>{|latex|\vspace{0.5\Cvs\}}

#@#  deref  is based on the  IDeref  protocol.  reset!  is based on the  IReset  protocol and  swap!  on  ISwap . We'll start by defining a data type and a constructor for our atom implementation:
@<code>{deref} は @<code>{IDeref}プロトコル、@<code>{reset!} は @<code>{IReset} プロトコル、@<code>{swap!} は @<code>{ISwap} プロトコルに基づいています。まず、atom の実装のために、データ型とコンストラクターを定義します。

//emlist{
(deftype MyAtom [^:mutable state ^:mutable watches]
  IPrintWithWriter
  (-pr-writer [p writer _]
    (-write writer (str "#<MyAtom " (pr-str state) ">"))))

(defn my-atom
  ([]
   (my-atom nil))
  ([init]
   (MyAtom. init {})))

(my-atom)　　　　;; => #<MyAtom nil>
(my-atom 42)　　;; => #<MyAtom 42>
//}

#@# Note that we've marked both the current state of the atom ( state ) and the map of watchers ( watches ) with the  {:mutable true}  metadata. We'll be modifying them and we're making this explicit with the annotations.
ここでは、atom の現在の状態と watcher のマップ(watches)の両方を {:mutable true} のメタデータでマークしていることに注目してください。これらを修正して、アノテーションを用いて明示的にします。

#@# Our  MyAtom  type is not very useful yet, we'll start by implementing the  IDeref  protocol so we can dereference its current value:
この @<code>{MyAtom} 型はまだあまり有用ではありませんが、まず @<code>{IDeref} プロトコルを実装して、現在の値を逆参照できるようにします。

//emlist{
(extend-type MyAtom
  IDeref
  (-deref [a]
    (.-state a)))

(def a (my-atom 42))

@a　　;; => 42
//}


#@# Page143
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# Now that we can dereference it we'll implement the  IWatchable  protocol, which will let us add and remove watches to our custom atom. We'll store the watches in the  watches  map of  MyAtom , associating keys to callbacks.
参照を解除できるようになったので、@<code>{IWatchable} プロトコルを実装して、独自の atom に watches を追加したり削除したりできるようにします。@<code>{watches} を @<code>{MyAtom} のマップに格納して、キーをコールバックに関連づけます。

//emlist{
(extend-type MyAtom
  IWatchable
  (-add-watch [a key f]
    (let [ws (.-watches a)]
      (set! (.-watches a) (assoc ws key f))))

  (-remove-watch [a key]
    (let [ws (.-watches a)]
      (set! (.-watches a) (dissoc ws key))))

  (-notify-watches [a oldval newval]
    (doseq [[key f] (.-watches a)]
      (f key a oldval newval))))
//}

#@# We can now add watches to our atom but is not very useful since we still can't change it. For incorporating change we have to implement the  IReset  protocol and make sure we notify the watches every time we reset the atom's value.
atom に watches を追加できるようになりましたが、まだ変更できないのであまり役に立ちません。変更を取り込むためには、@<code>{IReset} プロトコルを実装して、atom の値をリセットするたびに watches に通知する必要があります。

//emlist{
(extend-type MyAtom
  IReset
  (-reset! [a newval]
    (let [oldval (.-state a)]
      (set! (.-state a) newval)
      (-notify-watches a oldval newval)
      newval)))
//}

#@# Now let's check that we got it right. We'll add a watch, change the atom's value making sure the watch gets called and then remove it:
ここで、正しく動作するか確認しましょう。watches を追加してから atom の値を変更して、watches が呼び出されることを確認してから削除します。

//emlist{
(def a (my-atom 41))
;; => #<MyAtom 41>

(add-watch a :log (fn [key a oldval newval]
                    (println {:key key
                              :old oldval
                              :new newval})))
;; => #<MyAtom 41>

(reset! a 42)
;; {:key :log, :old 41, :new 42}
;; => 42
//}


#@# Page144
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(remove-watch a :log)
;; => #<MyAtom 42>

(reset! a 43)
;; => 43
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Our atom is still missing the swapping functionality so we'll add that now, let's implement the  ISwap  protocol. There are four arities for the  -swap!  method of the protocol since the function passed to  swap!  may take one, two, three or more arguments:
私たちが実装を進めている atom にはまだスワップの機能がないので、ここで ISwap プロトコルを実装します。-swap! には 4 つの arity があります。swap! に渡される関数には、1 つ以上の引数をとることができます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(extend-type MyAtom
  ISwap
  (-swap!
    ([a f]
     (let [oldval (.-state a)
           newval (f oldval)]
       (reset! a newval)))

    ([a f x]
     (let [oldval (.-state a)
           newval (f oldval x)]
       (reset! a newval)))

    ([a f x y]
     (let [oldval (.-state a)
           newval (f oldval x y)]
       (reset! a newval)))

    ([a f x y more]
     (let [oldval (.-state a)
           newval (apply f oldval x y more)]
       (reset! a newval)))))
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# We now have a custom implementation of the atom abstraction, let's test it in the REPL and see if it behaves like we expect:
これで、独自版の atom 抽象の実装ができました。これを REPL でテストし、期待どおりに動作するかどうかを確認しましょう。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(def a (my-atom 0))　　;; => #<MyAtom 0>

(add-watch a :log (fn [key a oldval newval]
                    (println {:key key
                              :old oldval
                              :new newval})))
;; => #<MyAtom 0>

(swap! a inc)
;; {:key :log, :old 0, :new 1}
;; => 1
//}


#@# Page145
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(swap! a + 2)
;; {:key :log, :old 1, :new 3}
;; => 3

(swap! a - 2)
;; {:key :log, :old 3, :new 1}
;; => 1

(swap! a + 2 3)
;; {:key :log, :old 1, :new 6}
;; => 6


(swap! a + 4 5 6)
;; {:key :log, :old 6, :new 21}
;; => 21

(swap! a * 2)
;; {:key :log, :old 21, :new 42}
;; => 42

(remove-watch a :log)  ;; => #<MyAtom 42>
//}

#@# We did it! We implemented a version of ClojureScript Atom without support for metadata nor validators, extending it to support such features is left as an exercise for the reader. Note that you'll need to modify the  MyAtom  type for being able to store metadata and a validator.
成功しました! メタデータやバリデータをサポートしないバージョンの Atom を実装しましたが、このような機能をサポートするように拡張することは、読者の皆さんの課題とします。メタデータとバリデーターを保存できるようにするには、MyAtom 型を変更する必要があります。

===== Volatile

#@# Volatiles are simpler than atoms in that they don't support watching for changes. All changes override the previous value much like the mutable variables present in almost every programming language. Volatiles are based on the  IVolatile  protocol that only defines a method for  vreset! , since  vswap!  is implemented as a macro.
#@# Let's start by creating our own volatile type and constructor:
Volatile は atom よりも単純で、変化の監視をサポートしません。大半のプログラミング言語に存在する可変変数のように、全ての変更は前の値を上書きします。Volatile は IVolatile プロトコルに基づいています。IVolatile プロトコルは、vreset! のためのメソッドのみを定義します。これは vswap! がマクロとして実装されているからです。
まず、独自の volatile 型とコンストラクタを作成します。

//emlist{
(deftype MyVolatile [^:mutable state]
  IPrintWithWriter
  (-pr-writer [p writer _]
    (-write writer (str "#<MyVolatile " (pr-str state) ">"))))

(defn my-volatile
  ([]
    (my-volatile nil))
  ([v]
    (MyVolatile. v)))

(my-volatile)     ;; => #<MyVolatile nil>

(my-volatile 42)  ;; => #<MyVolatile 42>
//}


#@# Page146
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


#@# Our  MyVolatile  still needs to support dereferencing and reseting it, let's implement  IDeref  and  IVolatile , which will enable use to use  deref ,  vreset!  and  vswap!  in our custom volatile:
私たちの @<code>{MyVolatile} は、参照の解除と再設定をサポートする必要があります。では、@<code>{IDeref} と @<code>{IVolatile} を実装しましょう。これにより、@<code>{MyVolatile} で @<code>{deref}、@<code>{vreset!}、@<code>{vswap!} が使えるようになります。


//emlist{
(extend-type MyVolatile
  IDeref
  (-deref [v]
    (.-state v))

  IVolatile
  (-vreset! [v newval]
    (set! (.-state v) newval)
    newval))

(def v (my-volatile 0))
;; => #<MyVolatile 42>

(vreset! v 1)
;; => 1

@v
;; => 1

(vswap! v + 2 3)
;; => 6

@v
;; => 6
//}


=== 変更

#@# Mutation

#@# In the xref:transients[section about transients] we learned about the mutable counterparts of the immutable and persistent data structures that ClojureScript provides. These data structures are mutable, and the operations on them end with a bang ( ! ) to make that explicit. As you may have guessed every operation on transients is based on protocols.
transient のセクションでは、ClojureScript が提供する不変で永続的なデータ構造に対応する可変のデータ構造について学びました。これらのデータ構造は可変であり、それらに対する関数は、可変であることを明示するために感嘆符(!)で終わります。あなたの想像通り、transient に対する全ての操作はプロトコルに基づいています。


===== 永続性から一時性(一時性から永続性)

#@# From persistent to transient and viceversa

#@# We've learned that we can transform a persistent data structure with the  transient  function, which is based on the  IEditableCollection  protocol; for transforming a transient data structure to a persistent one we use  persistent! , based on  ITransientCollection .
@<code>{IEditableCollection} プロトコルに基づく @<code>{transient} 関数を使って、永続的なデータ構造を変換できることを学びました。@<code>{transient} を永続的なデータ構造に変換するには、@<code>{ITransientCollection}に基づく @<code>{persistent!} を使います。

#@# Implementing immutable and persistent data structures and their transient counterparts is out of the scope of this book but we recommend taking a look at ClojureScript's data structure implementation if you are curious.
不変で永続的なデータ構造とそれらの一時的な対応物を実装することはこの本の範囲外ですが、興味があれば ClojureScript のデータ構造の実装を見ることをお勧めします。

===== Transient のベクタとセット

#@# Transient vectors and sets

#@# We've learned about most of the protocols for transient data structures but we're missing two:  ITransientVector  for using  assoc!  on transient vectors and  ITransientSet  for using  disj!  on transient sets.
transient のデータ構造のためのプロトコルの大部分は学びましたが、assoc! を transient のベクタに使うための ITransientVector と、disj! を transient のセットに使うための ITransientSet についてまだ取り上げていません。

#@# For illustrating the  ITransientVector  protocol we'll extend the JavaScript array type for making it an associative transient data structure:
@<code>{ITransientVector} プロトコルを説明するために、連想的な transient のデータ構造にするために、JavaScript の配列型を拡張してみます。


#@# Page147
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(extend-type array
  ITransientAssociative
  (-assoc! [arr key val]
    (if (number? key)
      (-assoc-n! arr key val)
      (throw (js/Error.
              "Array's key for assoc! must be a number."))))

  ITransientVector
  (-assoc-n! [arr n val]
    (.splice arr n 1 val)
    arr))

(def a #js [1 2 3]) 　;; => #js [1 2 3]
(assoc! a 0 42)　　　　;; => #js [42 2 3]
(assoc! a 1 43)　　　　;; => #js [42 43 3]
(assoc! a 2 44)　　　　;; => #js [42 43 44]
//}

#@# For illustrating the  ITransientSet  protocol we'll extend the ES6 Set type for making it a transient set, supporting the  conj! ,  disj!  and  persistent!  operations. Note that we've extended the Set type previously for being able to convert it to ClojureScript and we'll take advantage of that fact.
@<code>{ITransientSet}プロトコルを説明するために、ES6 の Set 型を拡張して、@<code>{conj!}、@<code>{disj!}、@<code>{persistent!} をサポートします。以前に Set 型を拡張して ClojureScript に変換できるようにしたことを思い出してください。

//emlist{
(extend-type js/Set
  ITransientCollection
  (-conj! [s v]
    (.add s v)
    s)

  (-persistent! [s]
   (js->clj s))

  ITransientSet
  (-disjoin! [s v]
    (.delete s v)
    s))

(def s (js/Set.))

(conj! s 1)
(conj! s 1)
(conj! s 2)
(conj! s 2)

(persistent! s)　　;; => #{1 2}

(disj! s 1)

(persistent! s)　　;; => #{2}
//}

#@# Page148
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}

== CSP (core.asyncを用いた場合)

#@# CSP (with core.async)

#@# CSP stands for Communicating Sequential Processes, which is a formalism for describing concurrent systems pioneered by C. A. R. Hoare in 1978. It is a concurrency model based on message passing and synchronization through channels.  An in-depth look at the theoretical model behind CSP is beyond the scope of this book; instead we'll focus on presenting the concurrency primitives that  core.async  offers.
CSP は Communicating Sequential Processes の略で、1978 年に C.A.R.Hoare によって開発された並行システムを記述するための形式であり、チャネルを介したメッセージの受け渡しと同期に基づく並行性のモデルです。CSP の背後にある理論的モデルの詳細については、この本では説明しません。その代わりに、core.async が提供する並行性に関わるプリミティブに焦点を当てます。

#@#  core.async  is not part of ClojureScript core but it's implemented as a library. Even though it is not part of the core language it's widely used. Many libraries build on top of the  core.async  primitives, so we think it is worth covering in the book. It's also a good example of the syntactic abstractions that can be achieved by transforming code with ClojureScript macros, so we'll jump right in. You'll need to have  core.async  installed to run the examples presented in this section.
@<code>{core.async} は ClojureScript core の一部ではありませんが、ライブラリとして実装されています。言語の一部ではありませんが、広く使用されています。多くのライブラリはcore.asynx のプリミティブの上に構築されています。そのため、本書で取り上げる価値はあると思います。これは、ClojureScript のマクロでコードを変換することにより実現できる構文的な抽象化の良い例でもあります。このセクションで説明する例を実行するには、core.async をインストールする必要があります。

=== チャンネル

#@# Channels

#@# Channels are like conveyor belts, we can put and take a single value at a time from them. They can have multiple readers and writers, and they are the fundamental message-passing mechanism of  core.async . In order to see how it works, we'll create a channel to perform some operations on it.
チャンネルはベルトコンベアのようなもので、一度に 1 つの値を入れたり出したりできます。複数の reader と writer を持つことができ、core.async の基本的なメッセージパッシングのメカニズムです。これがどのように動作するかを確認するために、いくつかの操作を実行するチャンネルを作成します。

//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(enable-console-print!)

(def ch (chan))

(take! ch #(println "Got a value:" %))
;; => nil

;; there is a now a pending take operation,
;; let's put something on the channel
(put! ch 42)
;; Got a value: 42
;; => 42
//}

#@# In the above example we created a channel  ch  using the  chan  constructor.  After that we performed a take operation on the channel, providing a callback that will be invoked when the take operation succeeds. After using  put!  to put a value on the channel the take operation completed and the  "Got a value: 42"  string was printed. Note that  put!  returned the value that was just put to the channel.
上記の例では、@<code>{chan} コンストラクタを使用してチャネル @<code>{ch} を作成しました。その後、チャンネルで take の操作を実行して、take の操作が成功したとき、呼び出されるコールバックを提供します。値をチャンネル上で設定するために put!　を使った後、take の操作が完了して、"Got a value: 42" という文字列が出力されます。put! は、チャンネルに設定されたばかりの値を返します。

#@# The  put!  function accepts a callback like  take!  does but we didn't provide any in the last example. For puts the callback will be called whenever the value we provided has been taken. Puts and takes can happen in any order, let's do a few puts followed by takes to illustrate the point:
@<code>{take!} とは異なり、@<code>{put!} 関数はコールバックを受け取りませんが、前の例では何も提供していません。put の場合、コールバックは指定した値が取得されるたびに呼び出されます。put と take は任意の順序で発生しますが。ポイントを説明するために、いくつか put と take を実行してみます。


#@# Page149
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(def ch (chan))

(put! ch 42 #(println "Just put 42"))　;; => true

(put! ch 43 #(println "Just put 43"))　;; => true

(take! ch #(println "Got" %))
;; Got 42
;; Just put 42
;; => nil

(take! ch #(println "Got" %))
;; Got 43
;; Just put 43
;; => nil
//}

#@# You may be asking yourself why the  put!  operations return  true . It signals that the put operation could be performed, even though the value hasn't yet been taken. Channels can be closed, which will cause the put operations to not succeed:
あなたはなぜ @<code>{put!} が @<code>{true} を返したか疑問に思っているかもしれません。まだ値が取得されていない場合でも、put の操作を実行できることを示します。チャンネルは閉じることができ、これにより put の操作が失敗します。

//emlist{
(require '[cljs.core.async :refer [chan put! close!]])

(def ch (chan))

(close! ch)　　;; => nil

(put! ch 42)　 ;; => false
//}

#@# The above example was the simplest possible situation but what happens with pending operations when a channel is closed? Let's do a few takes and close the channel and see what happens:
上記の例は最も単純な状況ですが、チャンネルが close されたとき、保留中の操作では何が起こるのでしょうか。いくつか take を実行してチャンネルを閉じて、何が起こるか見てみましょう。

//emlist{
(require '[cljs.core.async :refer [chan put! take! close!]])

(def ch (chan))

(take! ch #(println "Got value:" %))　　;; => nil
(take! ch #(println "Got value:" %))　　;; => nil

(close! ch)
;; Got value: nil
;; Got value: nil
;; => nil
//}


#@# Page150
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}

#@# We see that if the channel is closed all the  take!  operations receive a  nil  value.  nil  in channels is a sentinel value that signals to takers that the channel has been closed. Because of that, putting a  nil  value on a channel is not allowed:
チャンネルが閉じている場合は、すべての @<code>{take!} の操作は @<code>{nil} の値を受け取ります。チャンネルにおいて nil は、チャネルがクローズされたことを受信者に知らせるための指標値です。そのため、チャネルに @<code>{nil} の値を設定することはできません。

//emlist{
(require '[cljs.core.async :refer [chan put!]])

(def ch (chan))

(put! ch nil)
;; Error: Assert failed: Can't put nil in on a channel
//}

===== バッファ

#@# Buffers

#@# We've seen that pending take and put operations are enqueued in a channel but, what happens when there are many pending take or put operations? Let's find out by hammering a channel with many puts and takes:
保留中の take と put の操作は、チャンネルで enqueue されていますが、保留中の take の操作や put の操作が多数ある場合はどうなるでしょうか。put と take の多いチャンネルを叩いて確認してみましょう。

//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(def ch (chan))

(dotimes [n 1025]
  (put! ch n))
;; Error: Assert failed: No more than 1024
;; pending puts are allowed on a single channel.

(def ch (chan))

(dotimes [n 1025]
  (take! ch #(println "Got" %)))
;; Error: Assert failed: No more than 1024
;; pending takes are allowed on a single channel.
//}

#@# As the example above shows there's a limit of pending puts or takes on a channel, it's currently 1024 but that is an implementation detail that may change. Note that there can't be both pending puts and pending takes on a channel since puts will immediately succeed if there are pending takes and viceversa.
上記の例が示しているように、チャンネルには保留中の put または take には制限があり、現在は1024ですが、これは変更される可能性のある実装の詳細です。チャンネルには保留中の put と take の両方が存在することはできません。保留中の take がある場合、またはその逆の場合、put はすぐに成功するためです。

#@# Channels support buffering of put operations. If we create a channel with a buffer the put operations will succeed immediately if there's room in the buffer and be enqueued otherwise. Let's illustrate the point creating a channel with a buffer of one element. The  chan  constructors accepts a number as its first argument which will cause it to have a buffer with the given size:
チャンネルは put の操作のバッファリングをサポートします。バッファーを持つチャンネルを作成する場合、バッファーに空きがあれば put の操作は即座に成功して、そうでなければキューに入れられます。1 つの要素のバッファでチャンネルを作成するポイントを説明します。chan コンストラクタは最初の引数として数値を受け取り、指定されたサイズのバッファをもちます。


#@# Page151
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(def ch (chan 1))

(put! ch 42 #(println "Put succeeded!"))
;; Put succeeded!
;; => true

(dotimes [n 1024]
  (put! ch n))
;; => nil

(put! ch 42)
;; Error: Assert failed: No more than 1024
;; pending puts are allowed on a single channel.
//}

#@# What happened in the example above? We created a channel with a buffer of size 1 and performed a put operation on it that succeeded immediately because the value was buffered. After that we did another 1024 puts to fill the pending put queue and, when trying to put one value more the channel complained about not being able to enqueue more puts.
上の例では何が起こったのでしょうか。サイズ 1 のバッファーをもつチャンネルを作成して、そのチャンネルに対して put の操作を実行しましたが、値がバッファーに入れられたため、すぐに成功しました。その後、保留中の put の queue を埋めるためにさらに put の操作を 1024 回行い、さらに 1 つの値を put しようとすると、チャンネルはそれ以上の put を enqueue できないと警告を発しました。

#@# Now that we know about how channels work and what are buffers used for let's explore the different buffers that  core.async  implements. Different buffers have different policies and it's interesting to know all of them to know when to use what. Channels are unbuffered by default.
チャンネルがどのように機能して、どのバッファがどのように使われるわかったところで、次に core.async が実装している異なるバッファについて見ていきましょう。それぞれのバッファには異なるポリシーがあり、それぞれのバッファがいつ何を使うべきかを知るのは興味深いことです。デフォルトでは、チャンネルはバッファされません。

===== Fixed

#@# Fixed

#@# The fixed size buffer is the one that is created when we give the  chan  constructor a number and it will have the size specified by the given number. It is the simplest possible buffer: when full, puts will be enqueued.
固定サイズのバッファは、@<code>{chan} コンストラクタに数値を指定したときに作成され、その数値で指定されたサイズになります。これは可能な限り最も単純なバッファで、いっぱいになると put が queue に入れられます。

#@# The  chan  constructor accepts either a number or a buffer as its first argument. The two channels created in the following example both use a fixed buffer of size 32:
@<code>{chan} コンストラクタは、最初の引数として数値またはバッファのいずれかを受け入れます。次の例で作成した 2 つのチャンネルは、どちらもサイズ 32 の固定バッファを使用します。

//emlist{
(require '[cljs.core.async :refer [chan buffer]])

(def a-ch (chan 32))

(def another-ch (chan (buffer 32)))
//}


===== Dropping

#@# Dropping

#@# The fixed buffer allows put operations to be enqueued. However, as we saw before, puts are still queued when the fixed buffer is full. If we wan't to discard the put operations that happen when the buffer is full we can use a dropping buffer.
固定バッファーを使用すると、put の操作を enqueue できます。しかし前に見たように、固定バッファがいっぱいになっても put はqueue に入れられます。バッファーがいっぱいになったときに発生する put 操作を破棄したくない場合は、dropping バッファを使用できます。

#@# Dropping buffers have a fixed size and, when they are full puts will complete but their value will be discarded. Let's illustrate the point with an example:
dropping バッファのサイズは固定されており、いっぱいになると書き込みは完了しますが、その値は破棄されます。例を見てみましょう。


#@# Page152
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan dropping-buffer put! take!]])

(def ch (chan (dropping-buffer 2)))

(put! ch 40)
;; => true
(put! ch 41)
;; => true
(put! ch 42)
;; => true

(take! ch #(println "Got" %))
;; Got 40
;; => nil
(take! ch #(println "Got" %))
;; Got 41
;; => nil
(take! ch #(println "Got" %))
;; => nil
//}

#@# We performed three put operations and the three of them succeded but, since the dropping buffer of the channel has size 2, only the first two values were delivered to the takers. As you can observe the third take is enqueued since there is no value available, the third put's value (42) was discarded.
3 つの put の操作を実行して、3 つとも成功しましたが、チャンネルの dropping バッファのサイズ が 2 であるため、最初の 2 つの値だけが taker に渡されました。利用できる値がないため 3 番目の take が enqueue されているのがわかりますが、3 番目の出力値(42)は破棄されています。

===== スライド

#@# Sliding

#@# The sliding buffer has the opposite policy than the dropping buffer. When full puts will complete and the oldest value will be discarded in favor of the new one.  The sliding buffer is useful when we are interested in processing the last puts only and we can afford discarding old values.
sliding バッファは、dropping バッファとは逆のポリシーを持ちます。許容値の put が完了すると、最も古い値は新しい値に置き換えられます。sliding バッファは、最後の put だけを処理して、古い値を破棄できる場合に便利です。

//emlist{
(require '[cljs.core.async :refer [chan sliding-buffer put! take!]])

(def ch (chan (sliding-buffer 2)))

(put! ch 40)
;; => true
(put! ch 41)
;; => true
(put! ch 42)
;; => true

(take! ch #(println "Got" %))
;; Got 41
;; => nil
(take! ch #(println "Got" %))
;; Got 42
;; => nil
(take! ch #(println "Got" %))
;; => nil
//}


#@# Page153
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# We performed three put operations and the three of them succeded but, since the sliding buffer of the channel has size 2, only the last two values were delivered to the takers. As you can observe the third take is enqueued since there is no value available since the first put's value was discarded.
3 つの put の操作を行い、そのうちの 3 つは成功しましたが、チャンネルの sliding バッファのサイズが 2 であるため、最後の 2 つの値だけが取得側に渡されました。ご覧のとおり、最初の put の値が破棄されてから値が使用できないため、3 番目の take が enqueue されます。

===== Transducer

#@# Transducers

#@# As mentioned in the section about transducers, putting values in a channel can be thought as a transducible process. This means that we can create channels and hand them a transducer, giving us the ability to transform the input values before being put in the channel.
transducer の節でも見たように、チャンネルに値を入れることは transducible なプロセスと捉えることができます。つまり、チャンネルを作成して transducer を渡すことで、入力値を変換してからチャンネルに入れることができます。

#@# If we want to use a transducer with a channel we must supply a buffer since the reducing function that will be modified by the transducer will be the buffer's add function. A buffer's add function is a reducing function since it takes a buffer and an input and returns a buffer with such input incorporated.
チャンネルを持つ transducer を使用したい場合、transducer によって修正される reducing 関数がバッファの add 関数となるので、バッファを供給しなければなりません。バッファの add 関数は、バッファと入力を受け取り、そのような入力を組み込みながらバッファを返すため reducing 関数です。

//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(def ch (chan 1 (map inc)))

(put! ch 41)　　;; => true

(take! ch #(println "Got" %))
;; Got 42
;; => nil
//}

#@# You may be wondering what happens to a channel when the reducing function returns a reduced value. It turns out that the notion of termination for channels is being closed, so channels will be closed when a reduced value is encountered:
reducing 関数が reduce された値を返したとき、チャンネルはどうなるのでしょうか。チャンネルを終了する概念はクローズされているため、reduce された値が検出されるとチャンネルはクローズされます。

//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(def ch (chan 1 (take 2)))

(take! ch #(println "Got" %))　　;; => nil
(take! ch #(println "Got" %))　　;; => nil
(take! ch #(println "Got" %))　　;; => nil

(put! ch 41)　　
;; => true
(put! ch 42)
;; Got 41
;; => true
(put! ch 43)
;; Got 42
;; Got nil
;; => false
//}


#@# Page154
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# We used the  take  stateful transducer to allow maximum 2 puts into the channel.  We then performed three take operations on the channel and we expect only two to receive a value. As you can see in the above example the third take got the sentinel  nil  value which indicates that the channel was closed. Also, the third put operation returned  false  indicating that it didn't take place.
チャンネルに最大 2 回の入力を許可するので、@<code>{take}、@<code>{stateful}、@<code>{transducer}を使用しました。次に、チャンネルで 3 つの take の操作を実行して、値を受け取るのは 2 つだけだと予想します。上の例でわかるように、3 番目の take はチャンネルが閉じていることを示す sentinel nil 値を取得します。また、3 回目のput の操作は false を返して、失敗したことを示します。

===== 例外処理

#@# Handling exceptions

#@# If adding a value to a buffer throws an exception  core.async  the operation will fail and the exception will be logged to the console. However, channel constructors accept a third argument: a function for handling exceptions.
バッファに値を追加するとして例外が発生した場合、@<code>{core.async}では操作が失敗し、例外がコンソールに記録されます。しかし、チャンネルのコンストラクターは 3 番目の引数(例外を処理する関数)を受け入れます。

#@# When creating a channel with an exception handler it will be called with the exception whenever an exception occurs. If the handler returns  nil  the operation will fail silently and if it returns another value the add operation will be retried with such value.
例外ハンドラを持つチャンネルを作成すると、例外が発生するたびに例外ハンドラが呼び出されます。ハンドラが @<code>{nil} を返した場合は操作は暗黙的に失敗して、別の値を返した場合はその値で add の操作が再試行されます。

//emlist{
(require '[cljs.core.async :refer [chan put! take!]])

(enable-console-print!)

(defn exception-xform
  [rfn]
  (fn [acc input]
    (throw (js/Error. "I fail!"))))

(defn handle-exception
  [ex]
  (println "Exception message:" (.-message ex))
  42)

(def ch (chan 1 exception-xform handle-exception))

(put! ch 0)
;; Exception message: I fail!
;; => true

(take! ch #(println "Got:" %))
;; Got: 42
;; => nil
//}

===== Offer と  Poll

#@# Offer and Poll

#@# We've learned about the two basic operations on channels so far:  put!  and  take! . They either take or put a value and are enqueued if they can't be performed immediately. Both functions are asynchronous because of their nature: they can succeed but be completed at a later time.
これまでに、チャンネルに関する 2 つの基本的な操作である @<code>{put!} と @<code>{take!}について学びました。それらは、値を取得または設定しますが、すぐに実行できない場合は enqueue されます。どちらの関数もその性質的に非同期であり、すぐに成功することができますが、後で完了することもできます。

#@#  core.async  has two synchronous operations for putting or taking values:  offer!  and  poll! . Let's see how they work through examples.
@<code>{core.async} には値の設定と取得のために 2 つの同期的な操作があります。@<code>{offer!} と @<code>{poll!} です。それらがどのように機能するかを例を見てみましょう。

#@#  offer!  puts a value in a channel if it's possible to do so immediately. It returns  true  if the channel received the value and  false  otherwise. Note that, unlike with  put! ,  offer!  cannot distinguish between closed and open channels.
@<code>{offer!} はすぐに値を設定できる場合は、チャンネルに値を設定します。チャンネルが値を受け取った場合は @<code>{true} を返し、それ以外の場合は @<code>{false} を返します。@<code>{put!} とは異なり、@<code>{offer!} は閉じたチャンネルと開いたチャンネルを区別できない点に注意してください。


#@# Page155
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan offer!]])

(def ch (chan 1))

(offer! ch 42)
;; => true

(offer! ch 43)
;; => false
//}

#@#  poll!  takes a value from a channel if it's possible to do so immediately. Returns the value if succesful and  nil  otherwise. Unlike  take! ,  poll!  cannot distinguish closed and open channels.
@<code>{poll!} がすぐに値を取得できる場合は、チャンネルから値を取得します。成功した場合は値を返し、それ以外の場合は @<code>{nil} を返します。@<code>{take!} とは異なり、@<code>{poll!} は閉じたチャンネルと開いたチャンネルを区別できません。

//emlist{
(require '[cljs.core.async :refer [chan offer! poll!]])

(def ch (chan 1))

(poll! ch)
;; => nil

(offer! ch 42)
;; => true

(poll! ch)
;; => 42
//}


=== プロセス

#@# Processes

#@# We learned all about channels but there is still a missing piece in the puzzle: processes.  Processes are pieces of logic that run independently and use channels for communication and coordination. Puts and takes inside a process will stop the process until the operation completes. Stopping a process doesn't block the only thread we have in the environments where ClojureScript runs. Instead, it will be resumed at a later time when the operation is waiting for being performed.
チャンネルについてはすべて学びましたが、プロセスについては詳しく取り上げていませんでした。プロセスは、独立して実行されて、通信と調整のためにチャンネルを使用するロジックです。プロセス内での put および take は、操作が完了するとプロセスが停止します。プロセスの停止により ClojureScript が実行される環境にあるスレッドだけがブロックされるわけではありません。代わりに、操作の実行を待機しているときに再開されます。

#@# Processes are launched using the  go  macro and puts and takes use the  <!  and  >!  placeholders. The  go  macro rewrites your code to use callbacks but inside  go  everything looks like synchronous code, which makes understanding it straightforward:
プロセスは @<code>{go} マクロを使って起動されて、@<code>{put} と @<code>{take} には @<code>{<!} および @<code>{>!} のプレースホルダをつけます。@<code>{go} マクロはコールバックを使用するようにコードを書き換えますが、go の内部ではすべてが同期的なコードに見えるため、理解が容易です。

//emlist{
(require '[cljs.core.async :refer [chan <! >!]])
(require-macros '[cljs.core.async.macros :refer [go]])
//}



#@# Page156
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(enable-console-print!)

(def ch (chan))

(go
  (println [:a] "Gonna take from channel")
  (println [:a] "Got" (<! ch)))

(go
  (println [:b] "Gonna put on channel")
  (>! ch 42)
  (println [:b] "Just put 42"))

;; [:a] Gonna take from channel
;; [:b] Gonna put on channel
;; [:b] Just put 42
;; [:a] Got 42
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}

#@# In the above example we are launching a process with  go  that takes a value from  ch  and prints it to the console. Since the value isn't immediately available it will park until it can resume. After that we launch another process that puts a value on the channel.
上記の例では、@<code>{ch} から値を取得してコンソールに出力するプロセスを@<code>{go}で起動しています。値がすぐに使用可能になるわけではないので、値が使用可能になるまで一時停止します。その後、チャンネルに価値をもたらす別のプロセスを開始します。

#@# Since there is a pending take the put operation succeeds and the value is delivered to the first process, then both processes terminate.
保留中の take があるため、put の操作は成功します。値が最初のプロセスに渡されると、両方のプロセスが終了します。

#@# Both  go  blocks run independently and, even though they are executed asynchronously, they look like synchronous code. The above go blocks are fairly simple but being able to write concurrent processes that coordinate via channels is a very powerful tool for implementing complex asynchronous workflows. Channels also offer a great decoupling of producers and consumers.
どちらの @<code>{go} ブロックも独立して実行されて非同期に実行されますが、同期的なコードに見えます。上記の go ブロックはかなり単純ですが、チャンネルを介して調整する並行プロセスを書けることは、複雑な非同期のワークフローを実装するために非常に強力なツールとなります。チャンネルはまた、producer と consumer を分離します。

#@# Processes can wait for an arbitrary amount of time too, there is a  timeout  function that return a channel that will be closed after the given amount of miliseconds. Combining a timeout channel with a take operation inside a go block gives us the ability to sleep:
プロセスは任意の時間だけ待つことができ、与えられたミリ秒後に閉じられるチャンネルを返す @<code>{timeout} 関数があります。go ブロック内で timeout チャンネルと take の操作を組み合わせると、スリープさせることができます。


//emlist{
(require '[cljs.core.async :refer [<! timeout]])
(require-macros '[cljs.core.async.macros :refer [go]])

(enable-console-print!)

(defn seconds
  []
  (.getSeconds (js/Date.)))

(println "Launching go block")

(go
  (println [:a] "Gonna take a nap" (seconds))
  (<! (timeout 1000))
  (println [:a] "I slept one second, bye!" (seconds)))
//}

@<embed>{|latex|\vspace{-0.3\Cvs\}}


#@# Page157
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(println "Block launched")

;; Launching go block
;; Block launched
;; [:a] Gonna take a nap 9
;; [:a] I slept one second, bye! 10
//}

#@# As we can see in the messages printed, the process does nothing for one second when it blocks in the take operation of the timeout channel. The program continues and after one second the process resumes and terminates.
出力されたメッセージからわかるように、プロセスは、timeout チャンネルの take の操作でブロックされたときには、1 秒間何もしません。プログラムは続行されて、1 秒後にプロセスが再開するか終了します。


===== 選択

#@# Choice

#@# Apart from putting and taking one value at a time inside a go block we can also make a non-deterministic choice on multiple channel operations using  alts! .   alts!  is given a series of channel put or take operations (note that we can also try to put and take in a channel at the same time) and only performs one as soon as is ready; if multiple operations can be performed when calling  alts!  it will do a pseudo random choice by default.
go ブロック内で一度に 1 つの値を設定や取得するだけでなく、alts! を使って複数のチャンネル操作に非決定的な選択をすることもできます。一連のチャンネルの put または take の操作が与えられ(チャンネルで put と take は同時に行うこともできます)、準備が整うとすぐに実行されます。複数の操作がalts! を呼び出すときに実行できれば、擬似的なランダムの選択を行います。

#@# We can easily try an operation on a channel and cancel it after a certain amount of time combining the  timeout  function and  alts! . Let's see how:
timeout 関数と ats! を組み合わせることで、簡単にチャンネルの操作を試して、一定時間後に取り消すことができます。その方法を見てみましょう。

//emlist{
(require '[cljs.core.async :refer [chan <! timeout alts!]])
(require-macros '[cljs.core.async.macros :refer [go]])

(enable-console-print!)

(def ch (chan))

(go
  (println [:a] "Gonna take a nap")
  (<! (timeout 1000))
  (println [:a] "I slept one second, trying to put a value on channel")
  (>! ch 42)
  (println [:a] "I'm done!"))

(go
  (println [:b] "Gonna try taking from channel")
  (let [cancel (timeout 300)
        [value ch] (alts! [ch cancel])]
    (if (= ch cancel)
      (println [:b] "Too slow, take from channel cancelled")
      (println [:b] "Got" value))))

;; [:a] Gonna take a nap
;; [:b] Gonna try taking from channel
;; [:b] Too slow, take from channel cancelled
;; [:a] I slept one second, trying to put a value on channel
//}


#@# Page158
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# In the example above we launched a go block that, after waiting for a second, puts a value in the  ch  channel. The other go block creates a  cancel  channel, which will be closed after 300 miliseconds. After that, it tries to read from both  ch  and  cancel  at the same time using  alts! , which will succeed whenever it can take a value from either of those channels. Since  cancel  is closed after 300 miliseconds,  alts!  will succeed since takes from closed channel return the  nil  sentinel. Note that  alts!  returns a two-element vector with the returned value of the operation and the channel where it was performed.
上の例では、1 秒待ってから @<code>{ch} チャンネルに値を入れる go ブロックを起動しました。もう一方の @<code>{go} ブロックは、300 ミリ秒後に閉じる @<code>{cancel} チャンネルを作成します。その後、@<code>{alts!} を使って @<code>{ch} からの読み込みとキャンセルを同時に行おうとします。これらいずれかのチャンネルから値を取得できる場合は、常に成功します。@<code>{cancel} は 300 ミリ秒後にクローズされて、閉じたチャンネルからの @<code>{take} が　@<code>{nil} sentinel を返すため、@<code>{alts!} は成功します。@<code>{alts!} が 2 つの要素からなるベクタを返することに注目してください。それらは、操作による返り値と、実行されたチャンネルを含みます。

#@# This is why we are able to detect whether the read operation was performed in the  cancel  channel or in  ch . I suggest you copy this example and set the first process timeout to 100 miliseconds to see how the read operation on  ch  succeeds.
このため、@<code>{candel} チャンネルで read の操作が行われたか、@<code>{ch} チャンネルで行われたかを検出することができます。この例をコピーして、最初のプロセスの timeout を 100 ミリ秒に設定して、ch チャンネルに対する read の操作がどのように成功するかを確認することをお勧めします。

#@# We've learned how to choose between read operations so let's look at how to express a conditional write operation in  alts! . Since we need to provide the channel and a value to try to put on it, we'll use a two element vector with the channel and the value for representing write operations.
read の操作間での選択方法を学んだので、@<code>{alt!} で条件付きの read の操作を表現する方法を見てみましょう。チャンネルとその上に置こうとする値を提供する必要があるため、チャンネルと write の操作を表す値を持つ 2 つの要素のベクタを使用します。

#@# Let's see an example:

//emlist{
(require '[cljs.core.async :refer [chan <! alts!]])
(require-macros '[cljs.core.async.macros :refer [go]])

(enable-console-print!)

(def a-ch (chan))
(def another-ch (chan))

(go
  (println [:a] "Take a value from  a-ch ")
  (println [:a] "Got" (<! a-ch))
  (println [:a] "I'm done!"))

(go
  (println [:b] "Take a value from  another-ch ")
  (println [:a] "Got" (<! another-ch))
  (println [:b] "I'm done!"))

(go
  (println [:c] "Gonna try putting in both channels simultaneously")
  (let [[value ch] (alts! [[a-ch 42]
                           [another-ch 99]])]
    (if (= ch a-ch)
      (println [:c] "Put a value in  a-ch ")
      (println [:c] "Put a value in  another-ch "))))

;; [:a] Take a value from  a-ch 
;; [:b] Take a value from  another-ch 
;; [:c] Gonna try putting in both channels simultaneously
;; [:c] Put a value in  a-ch 
;; [:a] Got 42
;; [:a] I'm done!
//}

#@# When running the above example only the put operation on the  a-ch  channel has succeeded. Since both channels are ready to take a value when the  alts!  occurs you may get different results when running this code.
上記の例を実行すると、@<code>{a-ch} チャンネルでの put の操作だけが成功します。両方のチャンネルは、値を取得する準備ができています。このコードを実行すると異なる結果が得られることがあります。


#@# Page159
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


===== 優先順位

#@# Priority

#@#  alts!  default is to make a non-deterministic choice whenever several operations are ready to be performed. We can instead give priority to the operations passing the  :priority  option to  alts! . Whenever  :priority  is  true , if more than one operation is ready they will be tried in order.
デフォルトで @<code>{alt!} は、複数の操作を実行する準備ができている場合は、常に非決定的な選択を行いますが、代わりに alts! に :priority のオプションを渡す操作を優先させることができます。:priority が true の場合、もし 複数の操作が準備できていれば、順番に試行されます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(require '[cljs.core.async :refer [chan >! alts!]])
(require-macros '[cljs.core.async.macros :refer [go]])

(enable-console-print!)

(def a-ch (chan))
(def another-ch (chan))

(go
  (println [:a] "Put a value on  a-ch ")
  (>! a-ch 42)
  (println [:a] "I'm done!"))

(go
  (println [:b] "Put a value on  another-ch ")
  (>! another-ch 99)
  (println [:b] "I'm done!"))

(go
  (println [:c] "Gonna try taking from both channels with priority")
  (let [[value ch] (alts! [a-ch another-ch] :priority true)]
    (if (= ch a-ch)
      (println [:c] "Got" value "from  a-ch ")
      (println [:c] "Got" value "from  another-ch "))))

;; [:a] Put a value on  a-ch 
;; [:a] I'm done!
;; [:b] Put a value on  another-ch 
;; [:b] I'm done!
;; [:c] Gonna try taking from both channels with priority
;; [:c] Got 42 from  a-ch 
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Since both  a-ch  and  another-ch  had a value to read when the  alts!  was executed and we set the  :priority  option to true,  a-ch  has preference. You can try deleting the  :priority  option and running the example multiple times to see that, without priority,  alts!  makes a non-deterministic choice.
@<code>{a-ch} と @<code>{another-ch} は両方とも、いつ @<code>{alts!} を read するかの値を持っており、@<code>{:priority} オプションを true に設定すると @<code>{a-ch} が優先されます。@<code>{:priority} オプションを削除してこの例を複数回実行すると、@<code>{alts!} は priority なしで非決定論的な選択をします。

===== デフォルト

#@# Defaults

#@# Another interesting bit of  alts!  is that it can return immediately if no operation is ready and we provide a default value. We can conditionally do a choice on the operations if and only if any of them is ready, returning a default value if it's not.
@<code>{alts!} についてもう一つの興味深いことに、操作の準備ができておらず、デフォルト値を指定すれば、すぐに戻ることができます。いずれかの操作が準備できている場合に限り、条件付きで操作を選択することができます。準備できていない場合にはデフォルト値を返します。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(require '[cljs.core.async :refer [chan alts!]])
(require-macros '[cljs.core.async.macros :refer [go]])

(def a-ch (chan))
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Page160
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{15mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(def another-ch (chan))

(go
  (println [:a] "Gonna try taking from any of the channels without blocking")
  (let [[value ch] (alts! [a-ch another-ch] :default :not-ready)]
    (if (and (= value :not-ready)
             (= ch :default))
      (println [:a] "No operation is ready, aborting")
      (println [:a] "Got" value))))
;; [:a] Gonna try taking from any of the channels without blocking
;; [:a] No operation is ready, aborting
//}

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# As you can see in the above example, if no operation is ready the value returned by  alts!  is the one we supplied after the  :default  key when calling it and the channel is the  :default  keyword itself.
上の例でわかるように、操作の準備ができていない場合は、@<code>{alts!} による返される値は、呼び出し時に @<code>{:default} キーの後に指定したものであり、@<code>{channel} は @<code>{:default} キーワード自体です。


=== コンビネーター

#@# Combinators

@<embed>{|latex|\vspace{-0.4\Cvs\}}

#@# Now that we're acquainted with channels and processes it's time to explore some interesting combinators for working with channels that are present in  core.async . This section includes a brief description of all of them together with a simple example of their usage.
チャンネルとプロセスを理解したところで、次に、core.syanc に存在するチャンネルを扱うための興味深いコンビネーターについて検討します。このセクションでは、これらすべてについて簡単に説明して、簡単な使用例を示します。

===== パイプ

#@# pipe

#@#  pipe  takes an input and output channels and pipes all the values put on the input channel to the output one. The output channel is closed whenever the source is closed unless we provide a  false  third argument:
@<code>{pipe} は入力チャンネルと出力チャンネルを取り、入力チャンネルに設定されたすべての値を出力チャンネルに渡します。3 番目の引数に @<code>{false} を指定しない限り、ソースが閉じられるたびに出力チャンネルが閉じられます。

@<embed>{|latex|\vspace{-0.4\Cvs\}}

//emlist{
(require '[cljs.core.async :refer [chan pipe put! <! close!]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def in (chan))
(def out (chan))

(pipe in out)

(go-loop [value (<! out)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (println [:a] "Waiting for a value")
      (recur (<! out)))))

(put! in 0)  ;; => true
(put! in 1)  ;; => true
(close! in)

;; [:a] Got 0　;; [:a] Waiting for a value
;; [:a] Got 1　;; [:a] Waiting for a value
;; [:a] I'm done!
//}


#@# Page161
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# In the above example we used the  go-loop  macro for reading values recursively until the  out  channel is closed. Notice that when we close the  in  channel the  out  channel is closed too, making the  go-loop  terminate.
上記の例では、@<code>{go-loop} マクロを使用して、@<code>{out} チャンネルが閉じられるまで値を繰り返し読み取ります。@<code>{in} チャンネルを閉じると、@<code>{out} チャンネルも閉じられて @<code>{go-loop} が終了します。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

===== pipeline-async

#@#  pipeline-async  takes a number for controlling parallelism, an output channel, an asynchronous function and an input channel. The asynchronous function has two arguments: the value put in the input channel and a channel where it should put the result of its asynchronous operation, closing the result channel after finishing. The number controls the number of concurrent go blocks that will be used for calling the asynchronous function with the inputs.
@<code>{pipeline-async} は ある数字を受け取りますが、その数字は、並列性の制御、出力チャンネル、非同期の関数、入力チャンネルのためのものです。非同期の関数には 2 つの引数があり、入力チャンネルに格納された値と、非同期操作の結果を格納するチャンネルのために使います。result チャンネルは終了後に閉じられます。その数は、入力を使用して非同期関数を呼び出すために使用される並行的に動作する go ブロックの数を制御します。

#@# The output channel will receive outputs in an order relative to the input channel, regardless the time each asynchronous function call takes to complete. It has an optional last parameter that controls whether the output channel will be closed when the input channel is closed, which defaults to  true .
出力チャンネルは、非同期関数の呼び出しが完了するまでに要する時間に関係なく、入力チャンネルに相対的な順序で出力を受け取ります。これにはオプションの last パラメータがあり、入力チャンネルが閉じられたときに出力チャンネルを閉じるかどうかを制御します。デフォルトは true です。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//emlist{
(require '[cljs.core.async :refer [chan pipeline-async put! <! close!]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def in (chan))
(def out (chan))
(def parallelism 3)

(defn wait-and-put [value ch]
  (let [wait (rand-int 1000)]
    (js/setTimeout (fn []
                     (println "Waiting" wait "miliseconds for value" value)
                     (put! ch wait)
                     (close! ch))
                   wait)))

(pipeline-async parallelism out wait-and-put in)

(go-loop [value (<! out)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (println [:a] "Waiting for a value")
      (recur (<! out)))))

(put! in 1)
(put! in 2)
(put! in 3)
(close! in)
;; Waiting 164 miliseconds for value 3
;; Waiting 304 miliseconds for value 2
;; Waiting 908 miliseconds for value 1
;; [:a] Got 908
;; [:a] Waiting for a value
;; [:a] Got 304
;; [:a] Waiting for a value
;; [:a] Got 164
;; [:a] Waiting for a value
;; [:a] I'm done!
//}


#@# Page162
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


===== pipeline

#@#  pipeline  is similar to  pipeline-async  but instead of taking and asynchronous function it takes a transducer instead. The transducer will be applied independently to each input.
pipeline @<code>{pipeline-async} に似ていますが、非同期の関数を取る代わりに transducer を使います。 transducer は各入力に独立して適用されます。

//emlist{
(require '[cljs.core.async :refer [chan pipeline put! <! close!]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def in (chan))
(def out (chan))
(def parallelism 3)

(pipeline parallelism out (map inc) in)

(go-loop [value (<! out)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (println [:a] "Waiting for a value")
      (recur (<! out)))))

(put! in 1)
(put! in 2)
(put! in 3)
(close! in)

;; [:a] Got 2
;; [:a] Waiting for a value
;; [:a] Got 3
;; [:a] Waiting for a value
;; [:a] Got 4
;; [:a] Waiting for a value
;; [:a] I'm done!
//}


===== split

#@# split  takes a predicate and a channel and returns a vector with two channels, the first of which will receive the values for which the predicate is true, the second will receive those for which the predicate is false. We can optionally pass a buffer or number for the channels with the third (true channel) and fourth (false channel) arguments.
@<code>{split} は述部とチャンネルを取り、2 つのチャンネルを持つベクタを返します。最初のチャンネルは述部が true の値を受け取り、2 番目のチャンネルは述部が false の値を受け取ります。オプションで 3 番目(true チャンネル)と4番目(false チャンネル)の引数を使って、チャンネルのバッファまたは数字を渡すことができます。

//emlist{
(require '[cljs.core.async :refer [chan split put! <! close!]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def in (chan))
(def chans (split even? in))
(def even-ch (first chans))
(def odd-ch (second chans))
//}


#@# Page163
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(go-loop [value (<! even-ch)]
  (if (nil? value)
    (println [:evens] "I'm done!")
    (do
      (println [:evens] "Got" value)
      (println [:evens] "Waiting for a value")
      (recur (<! even-ch)))))

(go-loop [value (<! odd-ch)]
  (if (nil? value)
    (println [:odds] "I'm done!")
    (do
      (println [:odds] "Got" value)
      (println [:odds] "Waiting for a value")
      (recur (<! odd-ch)))))

(put! in 0)
(put! in 1)
(put! in 2)
(put! in 3)
(close! in)
;; [:evens] Got 0
;; [:evens] Waiting for a value
;; [:odds] Got 1
;; [:odds] Waiting for a value
;; [:odds] Got 3
;; [:odds] Waiting for a value
;; [:evens] Got 2
;; [:evens] Waiting for a value
;; [:evens] I'm done!
;; [:odds] I'm done!
//}

===== reduce

#@#  reduce  takes a reducing function, initial value and an input channel. It returns a channel with the result of reducing over all the values put on the input channel before closing it using the given initial value as the seed.
 @<code>{reduce} は、reducing 関数、初期値、および input チャンネルをとります。指定された初期値を seed として使用して、input チャンネルに設定されたすべての値に reduce を行い、閉じたチャンネルを返します。

//emlist{
(require '[cljs.core.async :as async :refer [chan put! <! close!]])
(require-macros '[cljs.core.async.macros :refer [go]])

(def in (chan))

(go
  (println "Result" (<! (async/reduce + (+) in))))

(put! in 0)
(put! in 1)
(put! in 2)
(put! in 3)
(close! in)
;; Result: 6
//}


#@# Page164
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


===== onto-chan

#@#  onto-chan  takes a channel and a collection and puts the contents of the collection into the channel. It closes the channel after finishing although it accepts a third argument for specifying if it should close it or not. Let's rewrite the  reduce  example using  onto-chan :
@<code>{onto-chan} はチャンネルとコレクションを取得して、コレクションの内容をチャンネルに格納します。終了後にチャンネルを閉じますが、チャンネルを閉じるかどうかを指定する 3 番目の引数を受け入れます。@<code>{onto-chan} を用いて @<code>{reduce} の例を書きかえてみましょう。

//emlist{
(require '[cljs.core.async :as async :refer [chan put! <! close! onto-chan]])
(require-macros '[cljs.core.async.macros :refer [go]])

(def in (chan))

(go
  (println "Result" (<! (async/reduce + (+) in))))

(onto-chan in [0 1 2 3])

;; Result: 6
//}


===== to-chan

#@#  to-chan  takes a collection and returns a channel where it will put every value in the collection, closing the channel afterwards.
@<code>{to-chan} はコレクションを取得して、コレクション内のすべての値を格納するチャンネルを返し、その後、チャンネルを閉じます。

//emlist{
(require '[cljs.core.async :refer [chan put! <! close! to-chan]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def ch (to-chan (range 3)))

(go-loop [value (<! ch)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (println [:a] "Waiting for a value")
      (recur (<! ch)))))

;; [:a] Got 0
;; [:a] Waiting for a value
;; [:a] Got 1
;; [:a] Waiting for a value
;; [:a] Got 2
;; [:a] Waiting for a value
;; [:a] I'm done!
//}


===== merge

#@#  merge  takes a collection of input channels and returns a channel where it will put every value that is put on the input channels. The returned channel will be closed when all the input channels have been closed. The returned channel will be unbuffered by default but a number or buffer can be provided as the last argument.
@<code>{merge} は入力チャンネルのコレクションを取得して、入力チャンネルに設定されたすべての値を格納するチャンネルを返します。すべての入力チャンネルを閉じると、返されたチャンネルが閉じます。返されるチャンネルはデフォルトではバッファされませんが、最後の引数として数値またはバッファを指定できます。


#@# Page165
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan put! <! close! merge]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

(def in1 (chan))
(def in2 (chan))
(def in3 (chan))

(def out (merge [in1 in2 in3]))

(go-loop [value (<! out)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (println [:a] "Waiting for a value")
      (recur (<! out)))))

(put! in1 1)
(close! in1)
(put! in2 2)
(close! in2)
(put! in3 3)
(close! in3)

;; [:a] Got 3
;; [:a] Waiting for a value
;; [:a] Got 2
;; [:a] Waiting for a value
;; [:a] Got 1
;; [:a] Waiting for a value
;; [:a] I'm done!
//}


=== 高度な抽象化

#@# Higher-level abstractions

#@# We've learned the about the low-level primitives of  core.async  and the combinators that it offers for working with channels.  core.async  also offers some useful, higher-level abstractions on top of channels that can serve as building blocks for more advanced functionality.
@<code>{core.async} の低レベルなプリミティブと、それらがチャンネルと動作するために提供されているコンビネータについて学びました。さらに @<code>{core.async} は、より高度な機能の構成要素として機能するチャンネルの上に、役に立つ高レベルの抽象化を提供します。

===== Mult

#@# Mult

#@# Whenever we have a channel whose values have to be broadcasted to many others, we can use  mult  for creating a multiple of the supplied channel. Once we have a mult, we can attach channels to it using  tap  and dettach them using  untap . Mults also support removing all tapped channels at once with  untap-all .
値を多くの他のチャンネルにブロードキャストする必要があるチャンネルがある場合は、@<code>{mult} を使用して、指定されたチャンネルを複数作成できます。モジュールができると @<code>{tap} を用いてチャンネルに接続し、@<code>{untap} してチャンネルを切り離します。また、@<code>{mults} は @<code>{untap-all} を使用して、タップしたすべてのチャンネルを一度に削除することもできます。

#@# Every value put in the source channel of a mult is broadcasted to all the tapped channels, and all of them must accept it before the next item is broadcasted. For preventing slow takers from blocking the mult's values we must use buffering on the tapped channels judiciously.
mult のソースチャンネルに入力されたすべての値は、タップされたすべてのチャンネルにブロードキャストされて、次のアイテムがブロードキャストされる前にすべてのチャンネルがそれを受け入れる必要があります。遅い taker が mult の値をブロックするのを防ぐためには、タップしたチャンネルのバッファリングを慎重に使用する必要があります。

#@# Closed tapped channels are removed automatically from the mult. When putting a value in the source channels when there are still no taps such value will be dropped.
クローズタップしたチャンネルは自動的にミュートから解除されます。まだタップがない状態でソースチャンネルに値を設定すると、その値はドロップされます。


#@# Page166
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


//emlist{
(require '[cljs.core.async :refer [chan put! <! close! timeout mult tap]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

;; Source channel and mult
(def in (chan))
(def m-in (mult in))

;; Sink channels
(def a-ch (chan))
(def another-ch (chan))

;; Taker for  a-ch 
(go-loop [value (<! a-ch)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Got" value)
      (recur (<! a-ch)))))

;; Taker for  another-ch , which sleeps for 3 seconds between takes
(go-loop [value (<! another-ch)]
  (if (nil? value)
    (println [:b] "I'm done!")
    (do
      (println [:b] "Got" value)
      (println [:b] "Resting 3 seconds")
      (<! (timeout 3000))
      (recur (<! another-ch)))))

;; Tap the two channels to the mult
(tap m-in a-ch)
(tap m-in another-ch)

;; See how the values are delivered to  a-ch  and  another-ch 
(put! in 1)
(put! in 2)

;; [:a] Got 1
;; [:b] Got 1
;; [:b] Resting for 3 seconds
;; [:a] Got 2
;; [:b] Got 2
;; [:b] Resting for 3 seconds
//}


===== Pub-sub

#@# After learning about mults you could imagine how to implement a pub-sub abstraction on top of  mult ,  tap  and  untap  but since it's a widely used communication mechanism  core.async  already implements this functionality.
@<code>{mult} について学んだ後で、@<code>{mult} の上に @<code>{pub-sub} の抽象化を実装する方法を想像してみてください。@<code>{core.async} はすでにこの機能を実装しています。

#@# Instead of creating a mult from a source channel, we create a publication with  pub  giving it a channel and a function that will be used for extracting the topic of the messages.
source チャンネルから mult を作成する代わりに、@<code>{pub} にチャンネル、またメッセージのトピックを抽出するために使用する関数を指定して publicztion を作成します。

#@# We can subscribe to a publication with  sub , giving it the publication we want to subscribe to, the topic we are interested in and a channel to put the messages that have the given topic. Note that we can subscribe a channel to multiple topics.
@<code>{sub} を持つ publicztion を subscribe して、subscribe したいpublicztion、関心のあるトピック、そのトピックを持つメッセージを配置するチャンネルを指定できます。1 つのチャンネルを複数のトピックに subscribe できます。


#@# Page167
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@#  unsub  can be given a publication, topic and channel for unsubscribing such channel from the topic.  unsub-all  can be given a publication and a topic to unsubscribe every channel from the given topic.
@<code>{unsub} には、そのようなチャンネルをトピックから unsubscribe するための publication、トピック、チャンネルを指定できます。@<code>{unsub-all} には、publication とトピックを指定して、指定したトピックからすべてのチャンネルをunsubscribeできます。

//emlist{
(require '[cljs.core.async :refer [chan put! <! close! pub sub]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

;; Source channel and publication
(def in (chan))
(def publication (pub in :action))

;; Sink channels
(def a-ch (chan))
(def another-ch (chan))

;; Channel with  :increment  action
(sub publication :increment a-ch)

(go-loop [value (<! a-ch)]
  (if (nil? value)
    (println [:a] "I'm done!")
    (do
      (println [:a] "Increment:" (inc (:value value)))
      (recur (<! a-ch)))))

;; Channel with  :double  action
(sub publication :double another-ch)

(go-loop [value (<! another-ch)]
  (if (nil? value)
    (println [:b] "I'm done!")
    (do
      (println [:b] "Double:" (* 2 (:value value)))
      (recur (<! another-ch)))))

;; See how values are delivered to  a-ch  and  another-ch  depending on their action
(put! in {:action :increment :value 98})
(put! in {:action :double :value 21})

;; [:a] Increment: 99
;; [:b] Double: 42
//}


===== Mixer

#@# As we learned in the section about  core.async  combinators, we can use the  merge  function for combining multiple channels into one. When merging multiple channels, every value put in the input channels will end up in the merged channel. However, we may want more finer-grained control over which values put in the input channels end up in the output channel, that's where mixers come in handy.
@<code>{core.async} のセクションで学んだように、複数のチャンネルを 1 つに結合する @<code>{merge} 関数を使用できます。複数のチャンネルを merge する場合、入力チャンネルに入力されたすべての値はマージされたチャンネルになります。ただし、入力チャンネルに入力された値が出力チャンネルに出力されるようにするには、より細かく制御する必要がある場合があります。この場合、Mixer が便利です。

#@#  core.async  gives us the mixer abstraction, which we can use to combine multiple input channnels into an output channel. The interesting part of the mixer is that we can mute, pause and listen exclusively to certain input channels.
@<code>{core.async} では、複数の入力チャンネルを 1 つの出力チャンネルに結合するために使用できる Mixer の抽象化が提供されます。Mixer の面白いところは、特定の input チャンネルだけをミュート、一時停止、再生できることです。


#@# Page168
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# We can create a mixer given an output channel with  mix . Once we have a mixer we can add input channels into the mix using  admix , remove it using  unmix  or remove every input channel with  unmix-all .
@<code>{mix} で出力チャンネルを指定して Mixer を作成できます。Mixerができたら、@<code>{admix} を使って入力チャンネルを mix に追加したり、@<code>{unmix} を使って削除したり、@<code>{unmix-all} を使ってすべての入力チャンネルを削除したりできます。

#@# For controlling the state of the input channel we use the  toggle  function giving it the mixer and a map from channels to their states. Note that we can add channels to the mix using  toggle , since the map will be merged with the current state of the mix. The state of a channel is a map which can have the keys  :mute ,  :pause  and  :solo  mapped to a boolean.
入力チャンネルの状態を制御するために、Mixer とチャンネルからその状態へのマップを与える toggle 関数を使用します。マップは Mix の現在の状態とマージされるため、@<code>{toggle} を使用してチャンネルを Mix に追加できます。チャンネルの状態はマップで、boolean にマップされる @<code>{:mute} @<code>{:pause} @<code>{:sole} のキーを持つことができます

#@# Let's see what muting, pausing and soloing channels means:
チャンネルの mute、pause、sole の意味を見てみましょう。


#@# - A muted input channel means that, while still taking values from it, they won't be forwarded to the output channel. Thus, while a channel is muted, all the values put in it will be discarded.
#@# - A paused input channel means that no values will be taken from it. This means that values put in the channel won't be forwarded to the output channel nor discarded.
#@# - When soloing one or more channels the output channel will only receive the values   put in soloed channels. By default non-soloed channels are muted but we can use    solo-mode  to decide between muting or pausing non-soloed channels.

@<embed>{|latex|\vspace{0.7\Cvs\}}

- ミュートされた入力チャンネルは、値を取得している間は出力チャンネルに転送されないことを意味します。したがって、チャンネルがミュートされている間は、そのチャンネルに入力されているすべての値が破棄されます。

- 一時停止された input チャンネルは、そこから値を取得しないことを意味します。つまり、チャンネルに入力された値は出力チャンネルに転送されず、破棄されません。

- 1 つまたは複数のチャンネルを sole にすると、出力チャンネルは sole に設定されたチャンネルの値だけを受け取ります。デフォルトでは、sole されていないチャンネルはミュートされますが、sole されていないチャンネルをミュートするか一時停止するかは sole モードで決定できます。

@<embed>{|latex|\vspace{0.7\Cvs\}}

#@# That was a lot of information so let's see an example to improve our understanding. First of all, we'll set up a mixer with an  out  channel and add three input channels to the mix. After that, we'll be printing all the values received on the  out  channel to illustrate the control over input channels:
多くの情報があったので、理解を深めるための例を見てみましょう。まず、アウトチャンネルの Mixer を設定し、ミックスに 3 つの入力チャンネルを追加します。その後、出力チャンネルで受信したすべての値を出力して、入力チャンネルへの制御を見てみましょう。

@<embed>{|latex|\vspace{-0.5\Cvs\}}

//emlist{
(require '[cljs.core.async :refer [chan put! <! close! mix admix
                                   unmix toggle solo-mode]])
(require-macros '[cljs.core.async.macros :refer [go-loop]])

;; Output channel and mixer
(def out (chan))
(def mixer (mix out))

;; Input channels
(def in-1 (chan))
(def in-2 (chan))
(def in-3 (chan))

(admix mixer in-1)
(admix mixer in-2)
(admix mixer in-3)

;; Let's listen to the  out  channel and print what we get from it
(go-loop [value (<! out)]
  (if (nil? value)
    (println [:a] "I'm done")
    (do
      (println [:a] "Got" value)
      (recur (<! out)))))
//}

#@# Page169
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}

#@# By default, every value put in the input channels will be put in the  out  channel:
デフォルトでは、入力チャンネルに設定されたすべての値が出力チャンネルに設定されます。

//emlist{
(do
  (put! in-1 1)
  (put! in-2 2)
  (put! in-3 3))

;; [:a] Got 1
;; [:a] Got 2
;; [:a] Got 3
//}

#@# Let's pause the  in-2  channel, put a value in every input channel and resume  in-2 :
n-2 チャンネルを一時停止し、すべての input チャンネルに値を入力して、in-2 を再開します。

//emlist{
(toggle mixer {in-2 {:pause true}})
;; => true

(do
  (put! in-1 1)
  (put! in-2 2)
  (put! in-3 3))

;; [:a] Got 1
;; [:a] Got 3

(toggle mixer {in-2 {:pause false}})

;; [:a] Got 2
//}

#@# As you can see in the example above, the values put in the paused channels aren't discarded. For discarding values put in an input channel we have to mute it, let's see an example:
上記の例からわかるように、一時停止されたチャンネルに入力された値は破棄されません。入力チャンネルに入力された値を破棄するには、ミュートする必要があります。次に例を示します。

//emlist{
(toggle mixer {in-2 {:mute true}})
;; => true

(do
  (put! in-1 1)
  ;; out will never get this value since it's discarded
  (put! in-2 2)
  (put! in-3 3))

;; [:a] Got 1
;; [:a] Got 3

(toggle mixer {in-2 {:mute false}})
//}


#@# Page170
#@# @<embed>{|latex|\vspace{-0.4\Cvs\}}
//embed[latex]{
\enlargethispage{20mm}
//}
//embed[latex]{
\clearpage
//}


#@# We put a value  2  in the  in-2  channel and, since the channel was muted at the time, the value is discarded and never put into  out . Let's look at the third state a channel can be inside a mixer: solo.
@<code>{in-2} チャンネルに値 @<code>{2} を設定しました。その時点でチャンネルがミュートされていたため、この値が破棄されて @<code>{out} に入ることはありません。チャンネルが Mixer の中にある 3 番目の状態 sole を見てみましょう。

#@# As we mentioned before, soloing channels of a mixer implies muting the rest of them by default:
前述したように、ミキサーのチャンネルを sole にすると、デフォルトで残りのチャンネルがミュートされます。

//emlist{
(toggle mixer {in-1 {:solo true}
               in-2 {:solo true}})
;; => true

(do
  (put! in-1 1)
  (put! in-2 2)
  ;; out will never get this value since it's discarded
  (put! in-3 3)) 

;; [:a] Got 1
;; [:a] Got 2

(toggle mixer {in-1 {:solo false}
               in-2 {:solo false}})
//}

#@# However, we can set the mode the non-soloed channels will be in while there are soloed channels. Let's set the default non-solo mode to pause instead of the default mute:
ただし、sole のチャンネルがある間は、sole でないチャンネルのモードを設定できます。デフォルトの非 sole モードを、デフォルトの mute ではなく pause に設定します。

//emlist{
(solo-mode mixer :pause)
;; => true
(toggle mixer {in-1 {:solo true}
               in-2 {:solo true}})
;; => true

(do
  (put! in-1 1)
  (put! in-2 2)
  (put! in-3 3))

;; [:a] Got 1
;; [:a] Got 2

(toggle mixer {in-1 {:solo false}
               in-2 {:solo false}})

;; [:a] Got 3
//}
