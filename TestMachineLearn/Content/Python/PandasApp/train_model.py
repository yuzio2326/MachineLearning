import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
import os
import joblib

# 스크립트의 현재 위치를 기준으로 절대 경로 생성
script_dir = os.path.dirname(os.path.abspath(__file__))
# AI.py는 PandasApp 폴더 안에 있으므로, 상위 폴더로 이동하여 경로 구성
base_dir = os.path.dirname(os.path.dirname(os.path.dirname(script_dir)))
csv_path = os.path.join(base_dir, 'Saved', 'InputLogs.csv')
model_path = os.path.join(script_dir, 'input_predictor_model.joblib')


# CSV 파일 읽기
try:
    df = pd.read_csv(csv_path)

    # 특성(X)과 타겟(y) 변수 설정
    features = ['TimeStamp', 'LocationX', 'LocationY', 'LocationZ', 'FrameNumber']
    target = 'InputName'

    X = df[features]
    y = df[target]

    # 훈련 데이터와 테스트 데이터 분할
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # 랜덤 포레스트 모델 생성 및 훈련
    model = RandomForestClassifier(n_estimators=100, random_state=42)
    model.fit(X_train, y_train)

    # 모델 평가
    y_pred = model.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)

    print(f"모델 정확도: {accuracy}")

    # 훈련된 모델 저장
    joblib.dump(model, model_path)
    print(f"모델이 '{model_path}'에 저장되었습니다.")

except FileNotFoundError:
    print(f"오류: '{csv_path}' 에서 파일을 찾을 수 없습니다.")
except Exception as e:
    print(f"오류 발생: {e}")
