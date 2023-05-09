#include "CRTRenderer.h"

int main() {
	CRTRenderer renderer;
	std::string name = "scene";
	for (size_t i = 1; i < 6; i++) {
		std::string idx = std::to_string(i);
		renderer.loadCRTScene(name + idx + ".crtscene");
		CRTImage image = renderer.render();
		image.writeToPPMFile(name + idx);
	}
	renderer.loadCRTScene("a.crtscene");
	CRTImage image = renderer.render();
	image.writeToPPMFile("a");
	renderer.loadCRTScene("a_smooth.crtscene");
	image = renderer.render();
	image.writeToPPMFile("a_smooth");
	return 0;
}
