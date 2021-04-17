#!/bin/zsh

# book のビルド
docker run -v `pwd`:/work -it nuitsjp/mdview:2.5 'cd /work && review-pdfmaker config.yml'

# 日時でコミット
commitTime=`date +%Y_%m%d_%H:%M`
commitMessage="updated: ${commitTime}"
git add .
git commit -m ${commitMessage}

# master ブランチに push
git push -u origin master
echo ${commitMessage}