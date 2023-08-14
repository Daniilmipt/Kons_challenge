from behave import *
import subprocess


@given('подготовили тестовый файл "{file_path}"')
def step_impl(context, file_path):
    context.file_path = file_path


@when('ищем пару слов "{word_string}" на расстоянии, не превышающем {distance} слов')
def step_impl(context, word_string, distance):
    command = f'./count_pairs.exe {context.file_path} '
    word_array = word_string.split()
    for word in word_array:
        command += word + " "
    command += distance

    context.output = subprocess.check_output(command, shell=True).decode()


@when('запускаем приложение с неправильным путем к тестовому файлу')
def step_impl(context):
    command = f'./count_pairs.exe {context.file_path} {"q"} {"q"} {"0"}'
    context.output = subprocess.check_output(command, shell=True).decode()


@then('находим {count} такие пары в тестовом файле')
def step_impl(context, count):
    assert context.output == count


@then('получаем сообщение об ошибке "{error}"')
def step_impl(context, error):
    assert context.output == error
