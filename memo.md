Dockerでディレクトリー内で起動する

要点：docker run -v `pwd`:/work -it nuitsjp/mdview:2.5 /bin/bash

$ docker pull nuitsjp/mdview:2.5
$ cd ~/ここ

$ docker run -v `pwd`:/work -it nuitsjp/mdview:2.5 /bin/bash
# cd /work/src
# review-pdfmaker config.yml
