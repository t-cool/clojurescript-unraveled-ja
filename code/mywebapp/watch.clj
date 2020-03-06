(require '[cljs.build.api :as b])

(b/watch "src"
 {:output-to "main.js"
  :output-dir "out/"
  :source-map true
  :main 'mywebapp.core
  :optimizations :none})