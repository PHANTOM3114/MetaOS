#!/bin/bash
set -e

IMAGE_NAME="metaos-dev:latest"
CONTAINER_NAME="metaos-dev-container"

if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
  echo "Docker image '$IMAGE_NAME' not found. Building..."
  docker build -t $IMAGE_NAME .
fi

if docker ps -a --format '{{.Names}}' | grep -q "^${CONTAINER_NAME}$"; then
  echo "Cleaning up old container..."
  docker rm -f $CONTAINER_NAME >/dev/null
fi

echo "Starting privileged container '$CONTAINER_NAME'..."
docker run -d --name $CONTAINER_NAME --privileged \
  -v "$(pwd)":/home/developer/project \
  -v "$HOME/.cache/bazel":/home/developer/.cache/bazel \
  $IMAGE_NAME sleep infinity

echo "Starting D-Bus service inside the container..."
docker exec -u root $CONTAINER_NAME /etc/init.d/dbus start

sleep 2

echo "Executing command: bazel $@"
docker exec \
  --interactive --tty \
  --user developer \
  --workdir /home/developer/project \
  $CONTAINER_NAME bazel "$@"

echo "Stopping and removing container..."
docker stop $CONTAINER_NAME >/dev/null
docker rm $CONTAINER_NAME >/dev/null

echo "Done."