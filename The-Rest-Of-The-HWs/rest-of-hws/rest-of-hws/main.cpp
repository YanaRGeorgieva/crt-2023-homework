#include <chrono>
#include <memory>
#include <string>

#include "CRTRenderer.h"

int main() {
	CRTRenderer renderer;
	std::string name = "scene";
	for (size_t i = 0; i < 1; i++) {
		std::string idx = std::to_string(i);
		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
		renderer.loadCRTScene(name + idx + ".crtscene");
		CRTImage image = renderer.render();
		image.writeToPPMFile(name + idx);
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
		std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		const double durationInSeconds = duration.count() / 1'000'000.0;
		printf("Reading, rendering and writing of %s took %lf seconds.\n", (name + idx).c_str(), durationInSeconds);
	}
	return 0;
}
