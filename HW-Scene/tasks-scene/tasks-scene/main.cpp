#include "CRTRenderer.h"

/*Задача 1.

Генерирайте картинка по зададените файлове, които съдържат информация за 3D сцени.
Картинките трябва да се генерират с помощта на рейтрейсинг: генериране на камера лъчи и пресичането им с триъгълниците на обектите в сцените.
За parse-ване на файла може да се използва RapidJSON: https://github.com/Tencent/rapidjson/releases
Оцветете пикселите, в които се виждат триъгълници по ваш избор:
Scene 0: https://bit.ly/35HyDXo
Scene 1: https://bit.ly/3x58QUe
Scene 2: https://bit.ly/35HxvTB
Scene 3: https://bit.ly/3r7n0k9
Scene 4: https://bit.ly/3uZIf8u
*/

int main() {
	srand(time(NULL));
	CRTRenderer renderer;
	renderer.loadCRTScene("scene4.crtscene");
	renderer.renderInPPMFile("scene4");
	return 0;
}
