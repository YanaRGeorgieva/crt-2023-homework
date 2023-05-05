#include "CRTRenderer.h"

int main() {
	srand(time(NULL));
	CRTRenderer renderer;
	renderer.loadCRTScene("scene1.crtscene");
	renderer.setAlbedo(CRTColor(0.5, 0.2, 0.3));
	renderer.setShadowBias(0.0001f);
	CRTImage image = renderer.render();
	image.writeToPPMFile("scene1");
	return 0;
}
