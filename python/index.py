import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("cpp/cleaned_students.csv")

print("Данные загружены. Количество записей:", len(df))
print("Колонки:", list(df.columns))

# =====================================================
# 1. Гистограмма распределения экзаменационных баллов
# =====================================================
plt.figure(figsize=(10, 6))
plt.hist(df['exam_score'], bins=20, color='skyblue', edgecolor='black')
plt.title('Распределение экзаменационных баллов', fontsize=14)
plt.xlabel('Экзаменационный балл', fontsize=12)
plt.ylabel('Количество студентов', fontsize=12)
plt.grid(True, alpha=0.3)
plt.savefig('python/hist_exam_score.png', dpi=150)
plt.close()
print("График 1 сохранён: hist_exam_score.png")

# =====================================================
# 2. Scatter plot: часы учёбы vs экзаменационный балл
# =====================================================
plt.figure(figsize=(10, 6))
plt.scatter(df['study_hours'], df['exam_score'], alpha=0.5, color='blue')
plt.title('Зависимость балла от времени учёбы', fontsize=14)
plt.xlabel('Часы учёбы в день', fontsize=12)
plt.ylabel('Экзаменационный балл', fontsize=12)
plt.grid(True, alpha=0.3)
plt.savefig('python/scatter_study_vs_score.png', dpi=150)
plt.close()
print("График 2 сохранён: scatter_study_vs_score.png")

# =====================================================
# 3. Столбчатая диаграмма: средний балл по полу
# =====================================================
avg_gender = df.groupby('gender_code')['exam_score'].mean()

plt.figure(figsize=(8, 6))
colors = ["#ffb0d8", "#bfdfff", "#D4FFD3"]  
bars = plt.bar([0, 1, 2], avg_gender.values, color=colors, edgecolor='black')
plt.title('Средний балл по полу', fontsize=14)
plt.xlabel('Пол', fontsize=12)
plt.ylabel('Средний экзаменационный балл', fontsize=12)
plt.xticks([0, 1, 2], ['Женщины', 'Мужчины', 'Другое'], rotation=0)

for bar, value in zip(bars, avg_gender.values):
    plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.5,
             f'{value:.1f}', ha='center', fontsize=11)

plt.savefig('python/avg_by_gender.png', dpi=150)
plt.close()
print("График 3 сохранён: avg_by_gender.png")

# =====================================================
# 4. Столбчатая диаграмма: средний балл по качеству питания
# =====================================================
avg_diet = df.groupby('diet_code')['exam_score'].mean()

plt.figure(figsize=(8, 6))
colors = ["#ffffc5", "#ffc8f5", "#bcbbff"]  
bars = plt.bar([1, 2, 3], avg_diet.values, color=colors, edgecolor='black')
plt.title('Средний балл по качеству питания', fontsize=14)
plt.xlabel('Качество питания', fontsize=12)
plt.ylabel('Средний экзаменационный балл', fontsize=12)
plt.xticks([1, 2, 3], ['Плохое', 'Среднее', 'Хорошее (Good)'], rotation=0)

for bar, value in zip(bars, avg_diet.values):
    plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.5,
             f'{value:.1f}', ha='center', fontsize=11)

plt.savefig('python/avg_by_diet.png', dpi=150)
plt.close()
print("График 4 сохранён: avg_by_diet.png")

# =====================================================
# 5. Дополнительный график: тепловая карта корреляций
# =====================================================
plt.figure(figsize=(12, 10))

numeric_cols = ['study_hours', 'social_media', 'netflix', 'attendance',
                'sleep_hours', 'exercise', 'mental_health', 'exam_score']
corr_matrix = df[numeric_cols].corr()

im = plt.imshow(corr_matrix, cmap='coolwarm', vmin=-1, vmax=1)
plt.colorbar(im)

plt.xticks(range(len(numeric_cols)), numeric_cols, rotation=45, ha='right')
plt.yticks(range(len(numeric_cols)), numeric_cols)

for i in range(len(numeric_cols)):
    for j in range(len(numeric_cols)):
        plt.text(j, i, f'{corr_matrix.iloc[i, j]:.2f}',
                 ha='center', va='center', fontsize=9)

plt.title('Корреляция между различными факторами и успеваемостью', fontsize=14)
plt.tight_layout()
plt.savefig('python/correlation_heatmap.png', dpi=150)
plt.close()
print("График 5 (дополнительный) сохранён: correlation_heatmap.png")

# =====================================================
# Вывод статистики в консоль
# =====================================================
print("\n" + "="*50)
print("СТАТИСТИКА ПО ДАННЫМ")
print("="*50)

print(f"\nВсего студентов: {len(df)}")
print(f"Средний балл: {df['exam_score'].mean():.2f}")
print(f"Максимальный балл: {df['exam_score'].max():.2f}")
print(f"Минимальный балл: {df['exam_score'].min():.2f}")

print("\n--- Средний балл по времени учёбы ---")
study_groups = df.groupby(pd.cut(df['study_hours'], bins=[0, 2, 4, 6, 100]))
for group, data in study_groups:
    if len(data) > 0:
        if group.right == 100:
            print(f"  более 6 часов: {data['exam_score'].mean():.2f} (n={len(data)})")
        else:
            print(f"  {group.left}-{group.right} часа: {data['exam_score'].mean():.2f} (n={len(data)})")

print("\n--- Средний балл по качеству питания ---")
diet_names = {1: "Плохое", 2: "Среднее", 3: "Good (хорошее)"}
for code, name in diet_names.items():
    data = df[df['diet_code'] == code]
    if len(data) > 0:
        print(f"  {name}: {data['exam_score'].mean():.2f} (n={len(data)})")

print("\n--- Средний балл по полу ---")
gender_names = {0: "Женщины", 1: "Мужчины", 2: "Другое"}
for code, name in gender_names.items():
    data = df[df['gender_code'] == code]
    if len(data) > 0:
        print(f"  {name}: {data['exam_score'].mean():.2f} (n={len(data)})")

print("\n--- Средний балл по времени сна ---")
sleep_groups = df.groupby(pd.cut(df['sleep_hours'], bins=[0, 6, 7, 8, 24]))
for group, data in sleep_groups:
    if len(data) > 0:
        if group.right == 24:
            print(f"  более 8 часов: {data['exam_score'].mean():.2f} (n={len(data)})")
        else:
            print(f"  {group.left}-{group.right} часов: {data['exam_score'].mean():.2f} (n={len(data)})")

print("\n--- Средний балл по времени в соцсетях ---")
social_groups = df.groupby(pd.cut(df['social_media'], bins=[0, 2, 4, 24]))
for group, data in social_groups:
    if len(data) > 0:
        if group.right == 24:
            print(f"  более 4 часов: {data['exam_score'].mean():.2f} (n={len(data)})")
        else:
            print(f"  {group.left}-{group.right} часов: {data['exam_score'].mean():.2f} (n={len(data)})")

print("\n" + "="*50)
print("ВСЕ ГРАФИКИ СОХРАНЕНЫ В ПАПКУ python/")
print("="*50)