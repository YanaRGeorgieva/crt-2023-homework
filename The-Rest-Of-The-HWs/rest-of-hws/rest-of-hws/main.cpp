#include <chrono>
#include <memory>
#include <string>

#include "CRTRenderer.h"

int main() {
	srand(time(nullptr));

	CRTRenderer renderer;
	std::string name = "fang";
	renderer.loadCRTScene("fang1.crtscene");
	CRTCamera& camera = renderer.getCamera();

	CRTVector initialPosition = camera.getPosition();
	CRTMatrix initialRotation = camera.getRotation();
	size_t fovy = 0;
	bool fovCycle = false;
	for (size_t degs = 0; degs < 360; degs++) {
		// Turn axis 
		if (degs > 45) {
			std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
			CRTImage image = renderer.render();
			std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
			image.writeToPPMFile(".\\dino\\" + name + std::to_string(degs));
			std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
			const double durationInSeconds = duration.count() / 1'000'000.0;
			printf("Rendering of %s took %lf seconds.\n", (name).c_str(), durationInSeconds);
		}
		camera.setFov(90.0f - fovy);
		camera.turnAroundAxis(initialRotation, initialPosition, degs, Z_AXIS);
		if (fovy < 60 && !fovCycle) {
			fovy++;
		} else if (fovy == 60 && !fovCycle) {
			fovCycle = true;
			fovy--;
		} else if (fovy < 60 && fovCycle) {
			fovy--;
		} else if (fovy == 0 && fovCycle) {
			fovy++;
			fovCycle = false;
		}
	}
	return 0;
}

/*
	RED DRAGON SCENE
	CPU: AMD Ryzen 7 6800HS with Radeon Graphics
	Reading, rendering and writing of scene0 took 1691.457880 seconds. (Debug, no parallel, no AABB).
	Reading, rendering and writing of scene0 took 149.828921 seconds. (Release, no parallel, no AABB).
	Reading, rendering and writing of scene0 took 18.241563 seconds. (Release, parallel, no AABB).
	Reading, rendering and writing of scene0 took 9.533833 seconds. (Release, parallel, AABB).
	Reading, rendering and writing of scene0 took 1.353483 seconds. (Release, parallel, BVH tree for each object in scene).
	Only rendering is 0.279391 seconds. (Release, parallel, BVH tree for each object in scene).
*/