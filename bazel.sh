if [[ "$(docker images -q metaos-dev:latest 2> /dev/null)" == "" ]]; then
  echo "Docker image 'metaos-dev' not found. Building..."
  docker build -t metaos-dev .
fi

docker run --rm -it \
  -v "$(pwd)":/home/developer/project \
  -v "$HOME/.cache/bazel":/home/developer/.cache/bazel \
  metaos-dev \
  bazel "$@"