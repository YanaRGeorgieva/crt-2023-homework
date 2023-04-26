#include "CRTRenderer.h"

/*Задача 1.

Генерирайте картинки, с използването на ray tracing по зададените сцени, които съдържат информация за 3D сцени. 
В сцените има информация за осветление, което трябва да вземете предвид при “оцветяване” на пикселите, 
в които се виждат триъгълници. Експериментирайте с различни albedo стойности(не са зададени в сцените):
Scene 0: https://bit.ly/37aQAOI
Scene 1: https://bit.ly/3jtmgBw
Scene 2: https://bit.ly/3O7FBpQ
Scene 3: https://bit.ly/3LeHHT8
*/

int main() {
	srand(time(NULL));
	CRTRenderer renderer;
	renderer.loadCRTScene("scene1.crtscene");
	renderer.setAlbedo(CRTColor(0.7f));
	renderer.setShadowBias(0.0001f);
	CRTImage image = renderer.render();
	image.writeToPPMFile("scene1");
	return 0;
}
