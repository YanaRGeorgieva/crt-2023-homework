#include "CRTRenderer.h"

int main() {
	CRTRenderer renderer;
	std::string name = "scene";
	for (size_t i = 0; i < 9; i++) {
		std::string idx = std::to_string(i);
		renderer.loadCRTScene(name + idx + ".crtscene");
		CRTImage image = renderer.render();
		image.writeToPPMFile(name + idx);
	}
	renderer.loadCRTScene("testArtefacts.crtscene");
	CRTImage image = renderer.render();
	image.writeToPPMFile("testArtefacts");
	return 0;
}
