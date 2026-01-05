#include "three.h"

#include <iostream>

int main() {
  ThreeEngine engine;
  if (engine.Initialize(800, 600, "ThreeEngine")) {
    engine.LoadMeshAsset("resources/axes.obj");
    engine.Run();
  } else {
    std::cerr << "Failed to initialize engine" << std::endl;
  }
  return 0;
}