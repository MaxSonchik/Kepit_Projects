import tkinter as tk
from tkinter import messagebox, ttk
import random
import winsound
import time

class MonopolyBoard:
    def __init__(self, root):
        self.root = root
        self.root.title("Монополия")
        self.root.geometry("1000x800")
        self.root.resizable(True, True)

        # Типы клеток и их цвета
        self.cell_colors = {
            "brown": "#8B4513",
            "light_blue": "#87CEEB", 
            "pink": "#FF69B4",
            "orange": "#FFA500",
            "red": "#DC143C",
            "yellow": "#FFD700",
            "green": "#32CD32",
            "dark_blue": "#00008B",
            "railroad": "#2F4F4F",
            "utility": "#1E90FF",
            "chance": "#FF6347",
            "community": "#228B22",
            "tax": "#A52A2A",
            "special": "#FFFFFF",
        }

        # Полное сопоставление клеток с типами и цветами
        self.cell_configs = {
            0: {"type": "special", "color": "special", "name": "GO", "price": 0},
            1: {"type": "property", "color": "brown", "name": "Mediterranean Ave", "price": 60},
            2: {"type": "community", "color": "community", "name": "Community Chest", "price": 0},
            3: {"type": "property", "color": "brown", "name": "Baltic Ave", "price": 60},
            4: {"type": "tax", "color": "tax", "name": "Income Tax", "price": 200},
            5: {"type": "railroad", "color": "railroad", "name": "Reading Railroad", "price": 200},
            6: {"type": "property", "color": "light_blue", "name": "Oriental Ave", "price": 100},
            7: {"type": "chance", "color": "chance", "name": "Chance", "price": 0},
            8: {"type": "property", "color": "light_blue", "name": "Vermont Ave", "price": 100},
            9: {"type": "property", "color": "light_blue", "name": "Connecticut Ave", "price": 120},
            10: {"type": "special", "color": "special", "name": "Jail", "price": 0},
            11: {"type": "property", "color": "pink", "name": "St. Charles Place", "price": 140},
            12: {"type": "utility", "color": "utility", "name": "Electric Company", "price": 150},
            13: {"type": "property", "color": "pink", "name": "States Ave", "price": 140},
            14: {"type": "property", "color": "pink", "name": "Virginia Ave", "price": 160},
            15: {"type": "railroad", "color": "railroad", "name": "Pennsylvania Railroad", "price": 200},
            16: {"type": "property", "color": "orange", "name": "St. James Place", "price": 180},
            17: {"type": "community", "color": "community", "name": "Community Chest", "price": 0},
            18: {"type": "property", "color": "orange", "name": "Tennessee Ave", "price": 180},
            19: {"type": "property", "color": "orange", "name": "New York Ave", "price": 200},
            20: {"type": "special", "color": "special", "name": "Free Parking", "price": 0},
            21: {"type": "property", "color": "red", "name": "Kentucky Ave", "price": 220},
            22: {"type": "chance", "color": "chance", "name": "Chance", "price": 0},
            23: {"type": "property", "color": "red", "name": "Indiana Ave", "price": 220},
            24: {"type": "property", "color": "red", "name": "Illinois Ave", "price": 240},
            25: {"type": "railroad", "color": "railroad", "name": "B&O Railroad", "price": 200},
            26: {"type": "property", "color": "yellow", "name": "Atlantic Ave", "price": 260},
            27: {"type": "property", "color": "yellow", "name": "Ventnor Ave", "price": 260},
            28: {"type": "utility", "color": "utility", "name": "Water Works", "price": 150},
            29: {"type": "property", "color": "yellow", "name": "Marvin Gardens", "price": 280},
            30: {"type": "special", "color": "special", "name": "Go To Jail", "price": 0},
            31: {"type": "property", "color": "green", "name": "Pacific Ave", "price": 300},
            32: {"type": "property", "color": "green", "name": "North Carolina Ave", "price": 300},
            33: {"type": "community", "color": "community", "name": "Community Chest", "price": 0},
            34: {"type": "property", "color": "green", "name": "Pennsylvania Ave", "price": 320},
            35: {"type": "railroad", "color": "railroad", "name": "Short Line", "price": 200},
            36: {"type": "chance", "color": "chance", "name": "Chance", "price": 0},
            37: {"type": "property", "color": "dark_blue", "name": "Park Place", "price": 350},
            38: {"type": "tax", "color": "tax", "name": "Luxury Tax", "price": 100},
            39: {"type": "property", "color": "dark_blue", "name": "Boardwalk", "price": 400},
        }

        # Игроки и их позиции
        self.players = [
            {"name": "Игрок 1", "color": "red", "position": 0, "money": 1500, "properties": []},
            {"name": "Игрок 2", "color": "blue", "position": 0, "money": 1500, "properties": []}
        ]
        self.current_player = 0
        self.player_tokens = {}
        self.auto_mode = False  # Флаг автоматического режима

        # Владельцы свойств
        self.property_owners = {i: None for i in range(40) if self.cell_configs[i]["type"] in ["property", "railroad", "utility"]}

        # Карточки шанса и общественной казны
        self.chance_cards = [
            {"text": "Получите $50!", "action": "add_money", "amount": 50},
            {"text": "Платите $100 за ремонт", "action": "subtract_money", "amount": 100},
            {"text": "Перейдите на GO", "action": "go_to_go"},
            {"text": "Отправляйтесь в тюрьму", "action": "go_to_jail"},
            {"text": "Получите $100!", "action": "add_money", "amount": 100},
            {"text": "Платите $50 налогов", "action": "subtract_money", "amount": 50}
        ]
        
        self.community_chest_cards = [
            {"text": "Наследство! Получите $100", "action": "add_money", "amount": 100},
            {"text": "Оплатите обучение $50", "action": "subtract_money", "amount": 50},
            {"text": "Вы выиграли конкурс! Получите $25", "action": "add_money", "amount": 25},
            {"text": "Врачебные счета $100", "action": "subtract_money", "amount": 100},
            {"text": "Возврат налогов $75", "action": "add_money", "amount": 75},
            {"text": "Рождественский бонус $100", "action": "add_money", "amount": 100}
        ]

        # основная рамка
        main_frame = ttk.Frame(root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # рамка для информации
        info_frame = ttk.Frame(main_frame, width=300)
        info_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=(10,0))
        info_frame.pack_propagate(False)

        # Canvas для поля
        self.canvas = tk.Canvas(main_frame, bg="white", highlightbackground="black", highlightthickness=2)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Элементы управления в info_frame
        self.create_info_panel(info_frame)

        # Иконки
        self.icons = {}
        self.load_icons()

        self.cells = []
        self.canvas.bind('<Configure>', self.on_resize)

    def play_sound(self, sound_type):
        """Воспроизведение звуков"""
        try:
            sound_params = {
                'purchase': (1000, 300),  # Звук покупки
                'jail': (300, 500),       # Звук тюрьмы
                'jackpot': (1500, 800),   # Звук выигрыша
                'dice': (800, 200),       # Звук кубиков
                'next_turn': (600, 150)   # Звук смены хода
            }
            
            if sound_type in sound_params:
                frequency, duration = sound_params[sound_type]
                winsound.Beep(frequency, duration)
        except:
            pass  # Игнорируем ошибки звука

    def create_info_panel(self, parent):
        """Создает панель информации и управления"""
        # Информация о текущем игроке
        player_frame = ttk.LabelFrame(parent, text="Текущий игрок", padding=15)
        player_frame.pack(fill=tk.X, pady=(0, 15))

        self.player_var = tk.StringVar(value=self.players[self.current_player]["name"])
        player_label = ttk.Label(player_frame, textvariable=self.player_var, font=("Arial", 12, "bold"))
        player_label.pack()

        self.money_var = tk.StringVar(value=f"Деньги: ${self.players[self.current_player]['money']}")
        money_label = ttk.Label(player_frame, textvariable=self.money_var)
        money_label.pack()

        # Кнопки управления
        control_frame = ttk.LabelFrame(parent, text="Управление", padding=10)
        control_frame.pack(fill=tk.X, pady=(0, 15))

        ttk.Button(control_frame, text="Бросить кубики", command=self.roll_dice).pack(fill=tk.X, pady=2)
        ttk.Button(control_frame, text="Купить собственность", command=self.buy_property).pack(fill=tk.X, pady=2)
        ttk.Button(control_frame, text="Следующий ход", command=self.next_turn).pack(fill=tk.X, pady=2)

        # Кнопки автоматического режима
        auto_frame = ttk.LabelFrame(parent, text="Автоматический режим", padding=10)
        auto_frame.pack(fill=tk.X, pady=(10, 0))

        ttk.Button(auto_frame, text="▶️ Автоход", command=self.start_auto_mode).pack(fill=tk.X, pady=2)
        ttk.Button(auto_frame, text="⏹️ Стоп", command=self.stop_auto_mode).pack(fill=tk.X, pady=2)

        # Информация о клетке
        self.cell_info_frame = ttk.LabelFrame(parent, text="Информация о клетке", padding=10)
        self.cell_info_frame.pack(fill=tk.BOTH, expand=True)

        self.cell_name_var = tk.StringVar(value="Выберите клетку")
        cell_name_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_name_var, font=("Arial", 10, "bold"))
        cell_name_label.pack()

        self.cell_price_var = tk.StringVar(value="")
        cell_price_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_price_var)
        cell_price_label.pack()

        self.cell_owner_var = tk.StringVar(value="")
        cell_owner_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_owner_var)
        cell_owner_label.pack()

    def start_auto_mode(self):
        """Запуск автоматического режима"""
        self.auto_mode = True
        messagebox.showinfo("Авторежим", "Автоматический режим запущен!")
        self.auto_play()

    def stop_auto_mode(self):
        """Остановка автоматического режима"""
        self.auto_mode = False
        messagebox.showinfo("Авторежим", "Автоматический режим остановлен!")

    def auto_play(self):
        """Автоматическая игра"""
        if not self.auto_mode:
            return

        # Бросаем кубики
        self.roll_dice()
        
        # Ждем 2 секунды и переходим к следующему ходу
        self.root.after(2000, self.auto_next_turn)

    def auto_next_turn(self):
        """Автоматический переход хода"""
        if not self.auto_mode:
            return
            
        self.next_turn()
        
        # Ждем 1 секунду и продолжаем автоигру
        self.root.after(1000, self.auto_play)

    def load_icons(self):
        """Загружает иконки или создает замену"""
        icon_configs = {
            "GO": "🎯", "Jail": "🚓", "Chance": "❓", "Community Chest": "📦",
            "Free Parking": "🅿️", "Go To Jail": "⛓️", "railroad": "🚂", "utility": "💡"
        }
        
        for name, symbol in icon_configs.items():
            # Создаем простые текстовые иконки если нет изображений
            self.icons[name] = symbol

    def on_resize(self, event):
        """Перерисовывает поле при изменении размера"""
        self.board_size = min(event.width - 10, event.height - 10)
        self.cell_size = self.board_size // 11
        self.draw_board()
        self.draw_players()

    def draw_board(self):
        """Рисует игровое поле"""
        self.canvas.delete("all")
        self.cells = []

        for cell_idx in range(40):
            coords = self.calculate_cell_coordinates(cell_idx)
            if coords:
                self.draw_cell(cell_idx, *coords)

    def calculate_cell_coordinates(self, cell_idx):
        """Вычисляет координаты клетки на поле по часовой стрелке"""
        # Верхняя сторона (0-9): слева направо
        if 0 <= cell_idx < 10:
            x1 = cell_idx * self.cell_size
            y1 = 0
            x2 = x1 + self.cell_size
            y2 = self.cell_size
        # Правая сторона (10-19): сверху вниз
        elif 10 <= cell_idx < 20:
            x1 = self.board_size - self.cell_size
            y1 = (cell_idx - 10) * self.cell_size
            x2 = self.board_size
            y2 = y1 + self.cell_size
        # Нижняя сторона (20-29): справа налево
        elif 20 <= cell_idx < 30:
            x1 = self.board_size - (cell_idx - 19) * self.cell_size
            y1 = self.board_size - self.cell_size
            x2 = x1 + self.cell_size
            y2 = self.board_size
        # Левая сторона (30-39): снизу вверх
        elif 30 <= cell_idx < 40:
            x1 = 0
            y1 = self.board_size - (cell_idx - 29) * self.cell_size
            x2 = self.cell_size
            y2 = y1 + self.cell_size
        else:
            return None
            
        return (x1, y1, x2, y2)

    def draw_cell(self, cell_idx, x1, y1, x2, y2):
        """Рисует отдельную клетку"""
        config = self.cell_configs[cell_idx]
        color = self.cell_colors[config["color"]]
        
        # Рисуем основу клетки
        cell_id = self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="black", width=2)
        
        # Добавляем текст названия
        cell_name = config["name"]
        lines = self.wrap_text(cell_name, 12)
        
        text_y = y1 + 8
        text_ids = []
        for line in lines:
            text_id = self.canvas.create_text(
                x1 + self.cell_size // 2, text_y,
                text=line, font=("Arial", 6, "bold"),
                width=self.cell_size - 4, justify="center"
            )
            text_ids.append(text_id)
            text_y += 10
        
        # Добавляем цену если есть
        price_id = None
        if config["price"] > 0:
            price_id = self.canvas.create_text(
                x1 + self.cell_size // 2, y2 - 15,
                text=f"${config['price']}", font=("Arial", 7, "bold"),
                fill="black"
            )
            
        # Добавляем иконку для специальных клеток
        icon_id = None
        if config["name"] in self.icons:
            icon_id = self.canvas.create_text(
                x1 + self.cell_size // 2, y1 + self.cell_size // 2 + 5,
                text=self.icons[config["name"]], font=("Arial", 10),
                fill="black"
            )
        elif config["type"] in ["railroad", "utility"]:
            icon_id = self.canvas.create_text(
                x1 + self.cell_size // 2, y1 + self.cell_size // 2 + 5,
                text=self.icons[config["type"]], font=("Arial", 10),
                fill="black"
            )

        # Сохраняем информацию о клетке
        self.cells.append({
            "id": cell_id, "text_ids": text_ids, "price_id": price_id, "icon_id": icon_id,
            "coords": (x1, y1, x2, y2), "config": config, "index": cell_idx
        })

        # Привязываем события
        self.canvas.tag_bind(cell_id, "<Button-1>", lambda e, idx=cell_idx: self.on_cell_click(idx))
        for text_id in text_ids:
            self.canvas.tag_bind(text_id, "<Button-1>", lambda e, idx=cell_idx: self.on_cell_click(idx))

    def wrap_text(self, text, max_length):
        """Разбивает текст на строки"""
        words = text.split()
        lines = []
        current_line = []
        
        for word in words:
            if len(' '.join(current_line + [word])) <= max_length:
                current_line.append(word)
            else:
                if current_line:
                    lines.append(' '.join(current_line))
                current_line = [word]
        
        if current_line:
            lines.append(' '.join(current_line))
            
        return lines

    def draw_players(self):
        """Рисует фишки игроков на поле - теперь по часовой стрелке"""
        # Удаляем старые фишки
        for token_id in self.player_tokens.values():
            self.canvas.delete(token_id)
        
        self.player_tokens = {}
        
        # Рисуем фишки для каждого игрока
        for i, player in enumerate(self.players):
            cell_idx = player["position"]
            if 0 <= cell_idx < 40 and len(self.cells) > cell_idx:
                x1, y1, x2, y2 = self.cells[cell_idx]["coords"]
                
                # Смещаем фишки для избежания наложения
                # Для 2 игроков - размещаем по диагонали
                if len(self.players) == 2:
                    offset_x = 5 if i == 0 else -5
                    offset_y = 5 if i == 0 else -5
                else:
                    # Для большего количества игроков - по кругу
                    import math
                    angle = (2 * 3.14159 * i) / len(self.players)
                    offset_x = int(8 * math.cos(angle))
                    offset_y = int(8 * math.sin(angle))
                
                token_id = self.canvas.create_oval(
                    x1 + self.cell_size//2 - 6 + offset_x, 
                    y1 + self.cell_size//2 - 6 + offset_y,
                    x1 + self.cell_size//2 + 6 + offset_x, 
                    y1 + self.cell_size//2 + 6 + offset_y,
                    fill=player["color"], outline="black", width=2
                )
                self.player_tokens[player["name"]] = token_id

    def on_cell_click(self, cell_idx):
        """Обрабатывает клик по клетке"""
        if cell_idx < len(self.cells):
            cell = self.cells[cell_idx]
            config = cell["config"]
            
            # Обновляем информацию о клетке
            self.cell_name_var.set(config["name"])
            
            if config["price"] > 0:
                self.cell_price_var.set(f"Цена: ${config['price']}")
            else:
                self.cell_price_var.set("")
                
            # Показываем владельца если есть
            owner = self.property_owners.get(cell_idx)
            if owner:
                self.cell_owner_var.set(f"Владелец: {owner}")
            else:
                self.cell_owner_var.set("Свободно")

    def roll_dice(self):
        """Бросок кубиков - каждый кубик от 1 до 6"""
        self.play_sound('dice')  # Звук кубиков
        
        dice1 = random.randint(1, 6)
        dice2 = random.randint(1, 6)
        total = dice1 + dice2
        
        messagebox.showinfo("Бросок кубиков", f"Выпало: {dice1} + {dice2} = {total}")
        
        # Перемещаем игрока
        self.move_player(total)

    def move_player(self, steps):
        """Перемещает текущего игрока по часовой стрелке"""
        player = self.players[self.current_player]
        old_position = player["position"]
        new_position = (player["position"] + steps) % 40
        player["position"] = new_position
        
        # Логируем перемещение
        print(f"{player['name']} перемещается с {old_position} на {new_position}")
        
        self.draw_players()
        self.on_cell_click(new_position)  # Показываем информацию о новой клетке
        
        # Проверяем прохождение через GO
        if new_position < old_position:
            player["money"] += 200
            self.play_sound('jackpot')  # Звук выигрыша
            messagebox.showinfo("GO!", "Вы прошли через GO и получаете $200!")
            self.update_player_info()
        
        # Проверяем специальные клетки
        self.check_special_cell(new_position)

    def check_special_cell(self, position):
        """Проверяет специальные клетки"""
        config = self.cell_configs[position]
        player = self.players[self.current_player]
        
        if config["type"] == "tax":
            tax_amount = config["price"]
            player["money"] -= tax_amount
            self.update_player_info()
            messagebox.showinfo("Налог", f"Вы платите налог: ${tax_amount}")
            
        elif config["type"] == "chance":
            self.draw_chance_card()
            
        elif config["type"] == "community":
            self.draw_community_chest_card()
            
        elif config["name"] == "Go To Jail":
            player["position"] = 10  # Jail
            self.play_sound('jail')  # Звук тюрьмы
            self.draw_players()
            messagebox.showinfo("Тюрьма", "Отправляйтесь в тюрьму!")
            
        elif config["type"] in ["property", "railroad", "utility"]:
            owner = self.property_owners.get(position)
            if owner and owner != player["name"]:
                # Простая логика аренды - 10% от стоимости
                rent = int(config["price"] * 0.1)
                player["money"] -= rent
                # Находим владельца и даем ему деньги
                for p in self.players:
                    if p["name"] == owner:
                        p["money"] += rent
                        break
                messagebox.showinfo("Аренда", f"Вы платите аренду {owner}: ${rent}")
                self.update_player_info()

    def draw_chance_card(self):
        """Обрабатывает карточку шанса"""
        card = random.choice(self.chance_cards)
        messagebox.showinfo("Шанс", card["text"])
        self.process_card_action(card)

    def draw_community_chest_card(self):
        """Обрабатывает карточку общественной казны"""
        card = random.choice(self.community_chest_cards)
        messagebox.showinfo("Общественная казна", card["text"])
        self.process_card_action(card)

    def process_card_action(self, card):
        """Обрабатывает действие карточки"""
        player = self.players[self.current_player]
        
        if card["action"] == "add_money":
            player["money"] += card["amount"]
            if card["amount"] >= 100:
                self.play_sound('jackpot')  # Звук выигрыша для крупных сумм
            messagebox.showinfo("Получение денег", f"Вы получаете ${card['amount']}!")
            
        elif card["action"] == "subtract_money":
            player["money"] -= card["amount"]
            messagebox.showinfo("Оплата", f"Вы платите ${card['amount']}!")
            
        elif card["action"] == "go_to_go":
            player["position"] = 0
            player["money"] += 200
            self.play_sound('jackpot')  # Звук выигрыша
            self.draw_players()
            messagebox.showinfo("Перемещение", "Вы перемещаетесь на GO и получаете $200!")
            
        elif card["action"] == "go_to_jail":
            player["position"] = 10
            self.play_sound('jail')  # Звук тюрьмы
            self.draw_players()
            messagebox.showinfo("Тюрьма", "Отправляйтесь в тюрьму!")
        
        self.update_player_info()

    def buy_property(self):
        """Покупка собственности"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        if config["type"] not in ["property", "railroad", "utility"]:
            messagebox.showwarning("Ошибка", "Здесь нельзя купить собственность!")
            return
            
        if self.property_owners[position] is not None:
            messagebox.showwarning("Ошибка", "Эта собственность уже куплена!")
            return
            
        if player["money"] < config["price"]:
            messagebox.showwarning("Ошибка", "Недостаточно денег!")
            return
            
        # Покупаем
        player["money"] -= config["price"]
        self.property_owners[position] = player["name"]
        player["properties"].append(position)
        
        self.play_sound('purchase')  # Звук покупки
        
        self.update_player_info()
        self.on_cell_click(position)  # Обновляем информацию о клетке
        messagebox.showinfo("Покупка", f"Вы купили {config['name']} за ${config['price']}!")

    def next_turn(self):
        """Следующий ход"""
        self.play_sound('next_turn')  # Звук смены хода
        
        self.current_player = (self.current_player + 1) % len(self.players)
        self.update_player_info()
        
        # В автоматическом режиме не показываем сообщение
        if not self.auto_mode:
            messagebox.showinfo("Смена хода", f"Теперь ходит {self.players[self.current_player]['name']}")

    def update_player_info(self):
        """Обновляет информацию о игроке"""
        player = self.players[self.current_player]
        self.player_var.set(player["name"])
        self.money_var.set(f"Деньги: ${player['money']}")

# Запуск приложения
if __name__ == "__main__":
    root = tk.Tk()
    app = MonopolyBoard(root)
    root.mainloop()