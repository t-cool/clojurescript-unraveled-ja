# External dependencies

## Closure Module compatible library

src/myextmods/myclosuremodule.js

```js
goog.provide("myextmods.myclosuremodule");

goog.scope(function() {
  var module = myextmods.myclosuremodule;
  module.get_greetings = function() {
    return "Hello from google closure module.";
  };
});
```

```bash
$ ./scripts/repl
```

```clojure
(require '[myextmods.myclosuremodule :as cm])
(cm/get_greetings)
;; => "Hello from google closure module."
```


## CommonJS modules compatible libraries

src/myextmods/mycommonjsmodule.js

```js
function getGreetings() {
  return "Hello from commonjs module.";
}

exports.getGreetings = getGreetings;
```

```bash
$ ./scripts/repl
```

```clojure
(require '[myextmods.mycommonjsmodule :as cm])
(cm/getGreetings)
;; => "Hello from commonjs module."
```

## Legacy, module-less (global scope) libraries

src/myextmods/myglobalmodule.js

```js
function getGreetings() {
  return "Hello from global scope.";
}
```

```bash
$ ./scripts/repl
```

```clojure
(require 'myextmods.myglobalmodule)
(js/getGreetings)
;; => "Hello from global scope."
```


