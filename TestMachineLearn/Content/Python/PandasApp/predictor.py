import joblib
import os
import pandas as pd

class InputPredictor:
    def __init__(self):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        model_path = os.path.join(script_dir, 'input_predictor_model.joblib')
        self.model = joblib.load(model_path)

    def predict_input(self, timestamp, x, y, z, frame):
        # 입력 데이터를 DataFrame으로 변환
        data = {
            'TimeStamp': [timestamp],
            'LocationX': [x],
            'LocationY': [y],
            'LocationZ': [z],
            'FrameNumber': [frame]
        }
        df = pd.DataFrame(data)
        
        # 예측 수행
        prediction = self.model.predict(df)
        return prediction[0]

# 언리얼에서 호출할 함수
def get_prediction(timestamp, x, y, z, frame):
    predictor = InputPredictor()
    return predictor.predict_input(timestamp, x, y, z, frame)
