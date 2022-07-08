#!/bin/zsh
# book のビルド
docker run -v `pwd`:/work -it nuitsjp/mdview:2.5 'cd /work && review-pdfmaker config.yml'