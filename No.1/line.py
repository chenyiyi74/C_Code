def is_on_curve(a, b, p, x, y):
    if x is None or y is None:  # 无穷远点
        return True
    left = (y * y) % p
    right = (x * x * x + a * x + b) % p
    return left == right


def mod_inverse(k, p):
    if k == 0:
        raise ValueError("0 没有模逆元")
    # 扩展欧几里得算法
    old_r, r = k, p
    old_s, s = 1, 0
    while r != 0:
        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s
    if old_r != 1:
        raise ValueError(f"{k} 在模 {p} 下没有逆元")
    return old_s % p


def point_add(a, b, p, x1, y1, x2, y2):

    if not is_on_curve(a, b, p, x1, y1) or not is_on_curve(a, b, p, x2, y2):
        return -1, -1

    if (x1 is None or y1 is None) and (x2 is None or y2 is None):
        return None, None  

    if x1 is None or y1 is None:
        return x2, y2

    if x2 is None or y2 is None:
        return x1, y1

    if x1 == x2 and y1 == y2:
        if y1 == 0:
            return None, None  
        numerator = (3 * x1 * x1 + a) % p
        denominator = (2 * y1) % p
        try:
            inv_den = mod_inverse(denominator, p)
        except ValueError:
            return None, None
        lam = (numerator * inv_den) % p
    else:
        if x1 == x2:
            return None, None  
        numerator = (y2 - y1) % p
        denominator = (x2 - x1) % p
        try:
            inv_den = mod_inverse(denominator, p)
        except ValueError:
            return None, None
        lam = (numerator * inv_den) % p

    # 计算 x3 = lambda^2 - x1 - x2
    x3 = (lam * lam - x1 - x2) % p
    # 计算 y3 = lambda * (x1 - x3) - y1
    y3 = (lam * (x1 - x3) - y1) % p

    return x3, y3


# ========== 主程序：读取输入并输出结果 ==========
def main():
    # 读取输入
    a = int(input().strip())
    b = int(input().strip())
    p = int(input().strip())
    x1 = int(input().strip())
    y1 = int(input().strip())
    x2 = int(input().strip())
    y2 = int(input().strip())

    # 计算点的和
    x3, y3 = point_add(a, b, p, x1, y1, x2, y2)

    # 输出结果
    if x3 is None or y3 is None:
        print(-1)
        print(-1)
    else:
        print(x3)
        print(y3)


if __name__ == "__main__":
    main()