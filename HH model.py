import numpy as np
import matplotlib.pyplot as plt


class HHVoltageClamp:
    def __init__(self):
        # 参数设置
        self.g_Na = 120.0
        self.E_Na = 50.0
        # 初始静息电位
        self.V_rest = -65.0

    # 门控变量速率函数
    def alpha_m(self, V): return 0.1 * (V + 40.0) / (1.0 - np.exp(-(V + 40.0) / 10.0))

    def beta_m(self, V):  return 4.0 * np.exp(-(V + 65.0) / 18.0)

    def alpha_h(self, V): return 0.07 * np.exp(-(V + 65.0) / 20.0)

    def beta_h(self, V):  return 1.0 / (1.0 + np.exp(-(V + 35.0) / 10.0))

    def run_clamp(self, T=20, dt=0.01, clamp_v=0.0, step_time=5.0):
        t = np.arange(0, T, dt)

        # 预设膜电位序列：从 -65mV 阶跃到 clamp_v
        V = np.where(t < step_time, self.V_rest, clamp_v)

        m = np.zeros(len(t))
        h = np.zeros(len(t))
        I_Na = np.zeros(len(t))

        # 计算初始稳态值 (t=0)
        m[0] = self.alpha_m(V[0]) / (self.alpha_m(V[0]) + self.beta_m(V[0]))
        h[0] = self.alpha_h(V[0]) / (self.alpha_h(V[0]) + self.beta_h(V[0]))

        # 迭代计算
        for i in range(1, len(t)):
            v_cur = V[i - 1]

            # 更新门控变量 (使用前一时刻的 V)
            m[i] = m[i - 1] + (self.alpha_m(v_cur) * (1 - m[i - 1]) - self.beta_m(v_cur) * m[i - 1]) * dt
            h[i] = h[i - 1] + (self.alpha_h(v_cur) * (1 - h[i - 1]) - self.beta_h(v_cur) * h[i - 1]) * dt

            # 计算钠电流 I_Na = g_Na * m^3 * h * (V - E_Na)
            # 注意：电流方向定义，通常向内电流为负
            I_Na[i] = self.g_Na * (m[i] ** 3) * h[i] * (V[i] - self.E_Na)

        return t, V, m, h, I_Na


# 运行模拟
sim = HHVoltageClamp()
t, V, m, h, I_Na = sim.run_clamp(T=15, clamp_v=0.0, step_time=2.0)

# 绘图
fig, axes = plt.subplots(3, 1, figsize=(8, 10), sharex=True)

# 1. 膜电位阶跃图
axes[0].plot(t, V, 'k', lw=2)
axes[0].set_ylabel('Voltage (mV)')
axes[0].set_title('Voltage Clamp: -65mV to 0mV')
axes[0].grid(True)

# 2. 门控变量 m 和 h 图
axes[1].plot(t, m, 'b', label='m (Activation)', lw=2)
axes[1].plot(t, h, 'r', label='h (Inactivation)', lw=2)
axes[1].set_ylabel('Gating Variables')
axes[1].legend()
axes[1].grid(True)

# 3. 钠电流 I_Na 图
axes[2].plot(t, I_Na, 'g', lw=2)
axes[2].set_ylabel('I_Na (uA/cm^2)')
axes[2].set_xlabel('Time (ms)')
axes[2].invert_yaxis()  # 习惯上将内向电流向下画
axes[2].set_title('Sodium Current (I_Na)')
axes[2].grid(True)

plt.tight_layout()
plt.show()
