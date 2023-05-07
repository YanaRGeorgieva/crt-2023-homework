#include "CRTRenderer.h"

int main() {
	CRTRenderer renderer;
	std::string name = "scene";
	for (size_t i = 1; i < 2; i++) {
		std::string idx = std::to_string(i);
		renderer.loadCRTScene(name + idx + ".crtscene");
		CRTImage image = renderer.render();
		image.writeToPPMFile(name + idx);
	}
	//renderer.loadCRTScene("a.crtscene");
	//CRTImage image = renderer.render();
	//image.writeToPPMFile("a");
	return 0;
}
