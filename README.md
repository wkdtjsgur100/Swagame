 플레이스토어 URL : https://play.google.com/store/apps/details?id=com.jang.swag

- 게임 어플리케이션
 - 사용 엔진/언어 : cocos2d-x, C++
● 자세한 logic은 소스 코드에 주석을 첨부하거나, 직관적인 이름을 사용했습니다.
● 프로그램 시작은 Classes/AppDelegae.cpp의 ApplicationDidFinishLaunching() 함수 내 director->runWithScene(scene); 에서 시작합니다. MainScene부터 객체가 만들어져 run 되는 방식입니다.

 *** 참고사항
     CSV 파싱을 통해 체계적인 스테이지를 구성했습니다.
     기획자가 엑셀로 레벨을 디자인을 하면, 클라이언트 프로그램에서 불러와 레벨 디자인 대로 플레이 할수 있도록 했습니다.
     서버에서 데이터를 불러와서 게임을 업데이트 하는 방식으로 확장할 수 있습니다.
 *** 게임 BGM은 소마 홍보 영상 BGM입니다. 