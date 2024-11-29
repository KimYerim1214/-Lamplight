import serial
import time
import openai

# OpenAI API 키 설정
openai.api_key = 'your-openai-api-key'

# 아두이노와 연결
arduino = serial.Serial(port='COM4', baudrate=9600, timeout=.1)
time.sleep(2) # 아두이노 재시작 대기

def send_to_arduino(message):
    """아두이노에 데이터 전송"""
    arduino.write(bytes(message, 'utf-8'))
    time.sleep(30)

def get_response_length_from_chatgpt(prompt):
    """ChatGPT로부터 응답 받고, 그 길이를 반환"""
    try:
        response = openai.Completion.create(
            engine="text-davinci-002", # 사용할 엔진 버전에 따라 변경 가능
            prompt=prompt,
            temperature=0.7,
            max_tokens=150,
            top_p=1.0,
            frequency_penalty=0.0,
            presence_penalty=0.0
        )
        return len(response.choices[0].text.strip())
    except Exception as e:
        print(f"An error occurred: {e}")
        return 0

# 사용자의 질문 또는 프롬프트
user_prompt = "ChatGPT가 대답하는 문자 데이터의 양에 따라 LED의 깜빡이는 횟수를 조절하는 방법은?"

time.sleep(30)

response_length = get_response_length_from_chatgpt(user_prompt)

time.sleep(30)

# 응답의 길이를 깜빡임 횟수로 변환 (예: 문자 10자당 1번 깜빡임)
blink_times = response_length // 10

# 깜빡임 횟수를 아두이노에 전송 (최소 1회 깜빡임)
blink_times = max(1, blink_times) # 응답이 아주 짧은 경우에도 최소한 1번은 깜빡이게 함
send_to_arduino(str(blink_times))
