import subprocess
def run_echo(input_data):
    process = subprocess.Popen(
        ['/Users/danielbond/CLionProjects/Assignment5-PLPP/cmake-build-debug/Assignment5_PLPP'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()
def test_input():
    tests = [
        {
            "input": "1+1\n",
            "expected_output": "2"
        },
        {
            "input": "1 + 23*(4+5)/9 - 6\n",
            "expected_output": "18"
        },
        {
            "input": "max(min(3,2),pow(3,4))\n",
            "expected_output": "81"
        },
        {
            "input": "a = max(min(3,2),pow(3,4))\na/3",
            "expected_output": "27"
        }
    ]
    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]
        output, error = run_echo(input_data)
        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")
if __name__ == "__main__":
    test_input()