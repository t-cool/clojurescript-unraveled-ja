(require '[cljs.build.api :as b])

(b/build "src"
 {
  :optimizations :none
  :main 'myapp1.core
  :output-to "main.js"
  :output-dir "out"
  :target :nodejs
  :verbose true})