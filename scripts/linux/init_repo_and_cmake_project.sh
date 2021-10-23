# init repo, create 'project' folder and call cmake

if ! [ -d "../../extern/googletest" ]; then
  git submodule init
  git submodule update
  echo "Cloned submodules"
fi

if ! [ -d "../../project" ]; then
  mkdir -p "../../project"
  echo "Created ''project' folder"
fi

cd ../../project

cmake ..

