# 자동차 번호판 인식

- https://mind3002.blogspot.com/2016/01/cc-opencv-license-plates-recognition.html의 내용을 기반으로 제작했습니다.
- 자동차 번호판을 인식하기 위해 OCR을 이용하는 방법이 있지만 윤곽에 기반해 Detection하는 방법을 사용해 보았습니다.

# 간단 설명

- 원본 이미지를 GrayScale로 변환합니다.
- 이후 adaptiveThreshold를 이용하여 이미지를 binarization합니다. 이미지를 이진화 하는 이유는 Canny edge의 결과를 좀 더 명확하게 하고 싶어 사용했습니다.
- Canny를 실행해 edge를 찾습니다.
- Canny를 통해 찾은 edge를 findContours 함수를 사용해 윤곽을 찾아냅니다.
- 찾아낸 윤곽에 박스를 생성하고 박스간의 기울기가 수평에 가까운 부분을 자동차 번호판으로 인식하고 인식된 자동차 번호판의 이미지만 추출하여 return하고 사용자에게 보여줍니다

# 실행화면 

<h4>1.원본이미지</h4>

![오리지날](https://user-images.githubusercontent.com/29748479/62671675-c0c64200-b9d2-11e9-873a-3e46817f534f.PNG)

<h4>2.GrayScale변환</h4>

![1](https://user-images.githubusercontent.com/29748479/62671846-5cf04900-b9d3-11e9-825c-78287322610c.PNG)

<h4>3.Binarization</h4>

![2](https://user-images.githubusercontent.com/29748479/62671901-8b6e2400-b9d3-11e9-88aa-a783982921ed.PNG)

<h4>4.CannyEdge detection</h4>

![3](https://user-images.githubusercontent.com/29748479/62671918-a476d500-b9d3-11e9-8acb-f3b7f20d99f8.PNG)

<h4>5.findContours + contour박스처리</h4>

![4](https://user-images.githubusercontent.com/29748479/62671960-c83a1b00-b9d3-11e9-97bb-0b920f101e98.PNG)

<h4>6.번호판으로 인식된 부분 확인</h4>

![결과 바로전](https://user-images.githubusercontent.com/29748479/62671986-e3a52600-b9d3-11e9-81d2-3bfac43bc23b.PNG)

<h4>7.최종결과 반환</h4>

![결과](https://user-images.githubusercontent.com/29748479/62672011-ffa8c780-b9d3-11e9-8c26-01b05ef99781.PNG)

# 문제점

- 이미지를 촬영한 각도와 빛의 양에 따라 번호판 인식에 문제가 발생합니다.
- 각기 다른 이미지의 전처리 과정에서 ratio값을 조정하더라도 완변한 결과를 얻기 힘듭니다.

# Comment

자동차 번호판을 인식하는 방법은 이미 수도 없이 많이 존재합니다. 처음 언급했던 방법인 OCR을 이용하는 방법이 있고 머신러닝을 이용 방법 또한 존재 합니다.
이 방법들을 이용했을 경우 현재 사용한 방법 보다 더 좋은 결과를 얻을 수 있는것은 분명합니다. 하지만 학부 과정에서 배운 openCV의 함수들을 사용해 보고 싶었고 
기초적인 함수들로도 충분히 번호판 인식이 가능할것으로 예상되어 이 방법을 사용해 보았습니다. 인식률은 좋지 못 했지만 이러한 방법도 사용이 가능하다는 것을 알게 되었습니다.
