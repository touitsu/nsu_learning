import numpy as np

# Количество точек
n = 21

# Генерация точек
x = np.linspace(-1, 1, n)
y = np.abs(x)

# Инициализация полинома-суммы (коэффициенты от младшей степени к старшей)
poly = np.zeros(n)

# Вычисление коэффициентов полинома Лагранжа
for i in range(n):
    base_poly = np.array([1.0])  # Начальный полином для i-й точки
    for j in range(n):
        if j != i:
            # Линейный множитель (x - x_j)
            linear_poly = np.array([-x[j], 1])
            # Умножение текущего base_poly на линейный множитель
            base_poly = np.convolve(base_poly, linear_poly)
            # Деление на (x_i - x_j)
            base_poly /= (x[i] - x[j])
    # Умножение на y_i и добавление к общему полиному
    base_poly *= y[i]
    poly[:len(base_poly)] += base_poly

# Вывод коэффициентов (от x^0 до x^20)
print("Коэффициенты полинома Лагранжа (от младшей степени к старшей):")
for i, coeff in enumerate(poly):
    print(f"x^{i}: {coeff:.10e}")

# Функция для вычисления значения полинома в точке using Horner's method
def eval_poly(coeffs, x_val):
    value = 0
    for c in reversed(coeffs):
        value = value * x_val + c
    return value

# Проверка в исходных точках
print("\nПроверка во всех точках:")
max_error = 0
for i in range(n):
    xi = x[i]
    value = eval_poly(poly, xi)
    error = abs(value - y[i])
    max_error = max(max_error, error)
    print(f"x = {xi:.4f}, вычисленное = {value:.10f}, ожидаемое = {y[i]:.4f}, ошибка = {error:.2e}")

print(f"\nМаксимальная ошибка: {max_error:.2e}")

# Проверка в дополнительных точках
test_points = np.linspace(-1, 1, 100)
print("\nПроверка в промежуточных точках (первые 10):")
for i, xi in enumerate(test_points[:10]):
    value = eval_poly(poly, xi)
    expected = abs(xi)
    error = abs(value - expected)
    print(f"x = {xi:.4f}, вычисленное = {value:.10f}, ожидаемое = {expected:.4f}, ошибка = {error:.2e}")
