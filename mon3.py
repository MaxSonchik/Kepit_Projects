import tkinter as tk
from tkinter import messagebox, ttk, filedialog
import random
import json
import os
import time

class DiceAnimation:
    """Класс для анимации броска кубиков"""
    def __init__(self, canvas, center_x, center_y, size=80):
        self.canvas = canvas
        self.center_x = center_x
        self.center_y = center_y
        self.size = size
        self.dice1_value = 1
        self.dice2_value = 1
        self.animation_running = False
        
        # Создаем элементы для кубиков
        self.dice1_rect = canvas.create_rectangle(
            center_x - size*1.5, center_y - size//2,
            center_x - size*0.5, center_y + size//2,
            fill="white", outline="black", width=3, tags="dice"
        ) 
        self.dice1_text = canvas.create_text(
            center_x - size, center_y,
            text="1", font=("Arial", 24, "bold"),
            fill="black", tags="dice"
        )
        
        self.dice2_rect = canvas.create_rectangle(
            center_x + size*0.5, center_y - size//2,
            center_x + size*1.5, center_y + size//2,
            fill="white", outline="black", width=3, tags="dice"
        )
        self.dice2_text = canvas.create_text(
            center_x + size, center_y,
            text="1", font=("Arial", 24, "bold"),
            fill="black", tags="dice"
        )
        
        # Текст результата
        self.result_text = canvas.create_text(
            center_x, center_y + size,
            text="", font=("Arial", 16, "bold"),
            fill="darkblue", tags="dice"
        )
        
        self.canvas.itemconfigure("dice", state="hidden")
    
    def show(self):
        """Показать анимацию"""
        self.canvas.itemconfigure("dice", state="normal")
    
    def hide(self):
        """Скрыть анимацию"""
        self.canvas.itemconfigure("dice", state="hidden")
    
    def roll(self, dice1, dice2, callback=None):
        """Анимировать бросок кубиков"""
        if self.animation_running:
            return
        
        self.animation_running = True
        self.show()
        self.animate_roll(10, dice1, dice2, callback)
    
    def animate_roll(self, frames_left, final_dice1, final_dice2, callback):
        """Анимация вращения кубиков"""
        if frames_left > 0:
            # Случайные значения для анимации
            temp_dice1 = random.randint(1, 6)
            temp_dice2 = random.randint(1, 6)
            
            self.canvas.itemconfigure(self.dice1_text, text=str(temp_dice1))
            self.canvas.itemconfigure(self.dice2_text, text=str(temp_dice2))
            
            # Анимация "дрожания"
            offset = 3 if frames_left % 2 == 0 else -3
            self.canvas.move(self.dice1_rect, offset, 0)
            self.canvas.move(self.dice1_text, offset, 0)
            self.canvas.move(self.dice2_rect, -offset, 0)
            self.canvas.move(self.dice2_text, -offset, 0)
            
            self.canvas.after(80, lambda: self.animate_roll(
                frames_left - 1, final_dice1, final_dice2, callback
            ))
        else:
            # Устанавливаем финальные значения
            self.dice1_value = final_dice1
            self.dice2_value = final_dice2
            
            self.canvas.itemconfigure(self.dice1_text, text=str(final_dice1))
            self.canvas.itemconfigure(self.dice2_text, text=str(final_dice2))
            
            total = final_dice1 + final_dice2
            result_text = f"Выпало: {final_dice1} + {final_dice2} = {total}"
            self.canvas.itemconfigure(self.result_text, text=result_text)
            
            # Возвращаем кубики на место
            self.canvas.coords(self.dice1_rect,
                self.center_x - self.size*1.5, self.center_y - self.size//2,
                self.center_x - self.size*0.5, self.center_y + self.size//2
            )
            self.canvas.coords(self.dice1_text,
                self.center_x - self.size, self.center_y
            )
            self.canvas.coords(self.dice2_rect,
                self.center_x + self.size*0.5, self.center_y - self.size//2,
                self.center_x + self.size*1.5, self.center_y + self.size//2
            )
            self.canvas.coords(self.dice2_text,
                self.center_x + self.size, self.center_y
            )
            
            self.animation_running = False
            
            if callback:
                self.canvas.after(1500, callback)  # Задержка перед скрытием
    
    def get_values(self):
        """Получить значения кубиков"""
        return self.dice1_value, self.dice2_value

class StartMenu:
    def __init__(self, root, start_game_callback, continue_game_callback):
        self.root = root
        self.start_game_callback = start_game_callback
        self.continue_game_callback = continue_game_callback
        self.create_menu()

    def create_menu(self):
        """Создание стартового меню"""
        # Устанавливаем размер окна
        self.root.geometry("800x600")
        self.root.title("Monopoly - Start Menu")
        
        # Фон меню
        bg_color = "#2C3E50"
        button_color = "#3498DB"
        button_hover = "#2980B9"
        text_color = "#ECF0F1"
        
        # Создаем Canvas для фона
        self.canvas = tk.Canvas(self.root, bg=bg_color, highlightthickness=0)
        self.canvas.pack(fill=tk.BOTH, expand=True)
        
        # Заголовок игры
        title_frame = tk.Frame(self.canvas, bg=bg_color)
        title_frame.place(relx=0.5, rely=0.2, anchor="center")
        
        title_label = tk.Label(title_frame, 
                              text="🎲 MONOPOLY 🏠", 
                              font=("Arial", 48, "bold"),
                              fg="#F1C40F",
                              bg=bg_color)
        title_label.pack()
        
        subtitle_label = tk.Label(title_frame,
                                 text="Classic Board Game",
                                 font=("Arial", 18, "italic"),
                                 fg="#ECF0F1",
                                 bg=bg_color)
        subtitle_label.pack(pady=(10, 0))
        
        # Фрейм для кнопок
        button_frame = tk.Frame(self.canvas, bg=bg_color)
        button_frame.place(relx=0.5, rely=0.5, anchor="center")
        
        # Стилизация кнопок
        button_style = {
            "font": ("Arial", 16, "bold"),
            "width": 25,
            "height": 2,
            "bd": 0,
            "cursor": "hand2",
            "activebackground": button_hover,
            "fg": text_color
        }
        
        # Только три кнопки в меню
        start_button = tk.Button(button_frame,
                                text="🎮 НАЧАТЬ НОВУЮ ИГРУ",
                                bg=button_color,
                                command=self.start_new_game,
                                **button_style)
        start_button.pack(pady=15)
        
        continue_button = tk.Button(button_frame,
                                   text="📂 ПРОДОЛЖИТЬ СТАРУЮ ИГРУ",
                                   bg="#27AE60",
                                   command=self.continue_old_game,
                                   **button_style)
        continue_button.pack(pady=15)
        
        exit_button = tk.Button(button_frame,
                               text="🚪 ВЫЙТИ ИЗ ИГРЫ",
                               bg="#E74C3C",
                               command=self.exit_game,
                               **button_style)
        exit_button.pack(pady=15)
        
        # Добавляем эффект при наведении на кнопки
        self.setup_button_hover(start_button, button_color, button_hover)
        self.setup_button_hover(continue_button, "#27AE60", "#229954")
        self.setup_button_hover(exit_button, "#E74C3C", "#C0392B")
        
        # Информационная панель внизу
        info_frame = tk.Frame(self.canvas, bg=bg_color)
        info_frame.place(relx=0.5, rely=0.85, anchor="center")
        
        info_text = """💡 Упрощенное меню:
• Начать новую игру - для новой партии
• Продолжить старую игру - загрузить автосохранение
• Выйти из игры - завершить приложение"""
        
        info_label = tk.Label(info_frame,
                             text=info_text,
                             font=("Arial", 10),
                             fg="#BDC3C7",
                             bg=bg_color,
                             justify="center")
        info_label.pack()
        
        # Автозагрузка при наличии файла автосохранения
        self.auto_check_save_file()

    def setup_button_hover(self, button, normal_color, hover_color):
        """Настройка эффекта наведения на кнопке"""
        def on_enter(e):
            button['background'] = hover_color
        
        def on_leave(e):
            button['background'] = normal_color
        
        button.bind("<Enter>", on_enter)
        button.bind("<Leave>", on_leave)

    def auto_check_save_file(self):
        """Проверка наличия файла автосохранения"""
        if os.path.exists("autosave.monopoly"):
            try:
                with open("autosave.monopoly", 'r') as f:
                    game_data = json.load(f)
                if game_data:
                    # Показываем информацию о автосохранении
                    self.show_autosave_info()
            except:
                pass

    def show_autosave_info(self):
        """Показать информацию об автосохранении"""
        try:
            with open("autosave.monopoly", 'r') as f:
                game_data = json.load(f)
            
            if game_data:
                timestamp = game_data.get("timestamp", "Неизвестно")
                player_names = [player["name"] for player in game_data.get("players", [])]
                
                # Создаем всплывающую панель
                info_frame = tk.Frame(self.canvas, bg="#34495E", relief="raised", bd=2)
                info_frame.place(relx=0.5, rely=0.7, anchor="center")
                
                info_label = tk.Label(info_frame,
                                     text=f"📀 Найдено автосохранение\nДата: {timestamp}\nИгроки: {', '.join(player_names)}",
                                     font=("Arial", 10),
                                     fg="#ECF0F1",
                                     bg="#34495E",
                                     padx=20,
                                     pady=10)
                info_label.pack()
                
                # Кнопка для быстрой загрузки автосохранения
                quick_load_btn = tk.Button(info_frame,
                                          text="Быстрая загрузка автосохранения",
                                          font=("Arial", 9, "bold"),
                                          bg="#E67E22",
                                          fg="white",
                                          bd=0,
                                          cursor="hand2",
                                          command=lambda: self.continue_old_game(True))
                quick_load_btn.pack(fill=tk.X, pady=(0, 10), padx=10)
                
                def on_hover(e):
                    quick_load_btn['background'] = "#D35400"
                
                def off_hover(e):
                    quick_load_btn['background'] = "#E67E22"
                
                quick_load_btn.bind("<Enter>", on_hover)
                quick_load_btn.bind("<Leave>", off_hover)
                
        except:
            pass

    def start_new_game(self):
        """Начать новую игру"""
        response = messagebox.askyesno("Начать новую игру", 
                                      "Вы уверены, что хотите начать новую игру?\nНесохраненные данные будут утеряны.")
        if response:
            self.root.destroy()  # Закрываем меню
            start_new_game()  # Запускаем новую игру

    def continue_old_game(self, autosave=False):
        """Продолжить старую игру"""
        try:
            if autosave or os.path.exists("autosave.monopoly"):
                self.root.destroy()  # Закрываем меню
                continue_old_game("autosave.monopoly")  # Загружаем автосохранение
            else:
                messagebox.showinfo("Автосохранение не найдено", 
                                  "Файл автосохранения не найден.\nНачните новую игру.")
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось загрузить автосохранение:\n{str(e)}")

    def exit_game(self):
        """Выйти из игры"""
        response = messagebox.askyesno("Выход из игры", 
                                      "Вы уверены, что хотите выйти из игры?")
        if response:
            self.root.quit()

class MonopolyBoard:
    def __init__(self, root, load_from_file=None):
        self.root = root
        self.root.title("Monopoly")
        self.root.geometry("1200x900")
        self.root.resizable(True, True)
        
        # Cell types and colors
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
            "chance": "#FF4500",
            "community": "#228B22",
            "tax": "#A52A2A",
            "special": "#F5F5DC",
        }

        # Color groups for property development with build costs
        self.color_groups = {
            "brown": {"house_cost": 50, "hotel_cost": 50, "properties": [1, 3]},
            "light_blue": {"house_cost": 50, "hotel_cost": 50, "properties": [6, 8, 9]},
            "pink": {"house_cost": 100, "hotel_cost": 100, "properties": [11, 13, 14]},
            "orange": {"house_cost": 100, "hotel_cost": 100, "properties": [16, 18, 19]},
            "red": {"house_cost": 150, "hotel_cost": 150, "properties": [21, 23, 24]},
            "yellow": {"house_cost": 150, "hotel_cost": 150, "properties": [26, 27, 29]},
            "green": {"house_cost": 200, "hotel_cost": 200, "properties": [31, 32, 34]},
            "dark_blue": {"house_cost": 200, "hotel_cost": 200, "properties": [37, 39]},
        }

        # Complete cell configurations
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

        # Rent tables with progression
        self.rent_tables = {
            1: [2, 10, 30, 90, 160, 250],
            3: [4, 20, 60, 180, 320, 450],
            6: [6, 30, 90, 270, 400, 550],
            8: [6, 30, 90, 270, 400, 550],
            9: [8, 40, 100, 300, 450, 600],
            11: [10, 50, 150, 450, 625, 750],
            13: [10, 50, 150, 450, 625, 750],
            14: [12, 60, 180, 500, 700, 900],
            16: [14, 70, 200, 550, 750, 950],
            18: [14, 70, 200, 550, 750, 950],
            19: [16, 80, 220, 600, 800, 1000],
            21: [18, 90, 250, 700, 875, 1050],
            23: [18, 90, 250, 700, 875, 1050],
            24: [20, 100, 300, 750, 925, 1100],
            26: [22, 110, 330, 800, 975, 1150],
            27: [22, 110, 330, 800, 975, 1150],
            29: [24, 120, 360, 850, 1025, 1200],
            31: [26, 130, 390, 900, 1100, 1275],
            32: [26, 130, 390, 900, 1100, 1275],
            34: [28, 150, 450, 1000, 1200, 1400],
            37: [35, 175, 500, 1100, 1300, 1500],
            39: [50, 200, 600, 1400, 1700, 2000],
        }

        # Инициализация игроков (по умолчанию)
        self.players = [
            {"name": "Player 1", "color": "red", "position": 0, "money": 1500, "properties": [], "in_jail": False, "jail_turns": 0},
            {"name": "Player 2", "color": "blue", "position": 0, "money": 1500, "properties": [], "in_jail": False, "jail_turns": 0}
        ]
        
        self.current_player = 0
        self.player_tokens = {}
        self.auto_mode = False
        self.auto_paused = False
        self.go_passed_count = 0  # Счетчик прохождений GO
        self.auto_delay = 2100  # Фиксированная задержка в авторежиме (в миллисекундах)

        # Property owners
        self.property_owners = {}
        for i in range(40):
            if self.cell_configs[i]["type"] in ["property", "railroad", "utility"]:
                self.property_owners[i] = {
                    "owner": None, 
                    "houses": 0, 
                    "hotel": False,
                    "mortgaged": False
                }
            else:
                self.property_owners[i] = None

        # Cards
        self.chance_cards = [
            {"text": "Get $50!", "action": "add_money", "amount": 50},
            {"text": "Pay $100 for repairs", "action": "subtract_money", "amount": 100},
            {"text": "Go to GO", "action": "go_to_go"},
            {"text": "Go to Jail", "action": "go_to_jail"},
            {"text": "Get $100!", "action": "add_money", "amount": 100},
            {"text": "Pay $50 in taxes", "action": "subtract_money", "amount": 50}
        ]
        
        self.community_chest_cards = [
            {"text": "Inheritance! Get $100", "action": "add_money", "amount": 100},
            {"text": "Pay $50 for education", "action": "subtract_money", "amount": 50},
            {"text": "You won a contest! Get $25", "action": "add_money", "amount": 25},
            {"text": "Doctor's fees $100", "action": "subtract_money", "amount": 100},
            {"text": "Tax refund $75", "action": "add_money", "amount": 75},
            {"text": "Christmas bonus $100", "action": "add_money", "amount": 100}
        ]

        # Main GUI setup
        main_frame = ttk.Frame(root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        info_frame = ttk.Frame(main_frame, width=400)
        info_frame.pack(side=tk.RIGHT, fill=tk.Y, padx=(10, 0))
        info_frame.pack_propagate(False)

        self.canvas = tk.Canvas(main_frame, bg="#F0F0F0", highlightbackground="black", highlightthickness=3)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        # Анимация кубиков
        self.dice_animation = None
        
        self.create_info_panel(info_frame)
        self.icons = {}
        self.load_icons()
        self.cells = []
        self.canvas.bind('<Configure>', self.on_resize)
        
        # Если указан файл для загрузки, загружаем игру
        if load_from_file:
            self.load_game_on_start(load_from_file)
        else:
            # Initial draw
            self.root.update()
            self.draw_board()
            self.draw_players()
        
        # Добавляем меню "Файл"
        self.create_menu_bar()

    def create_menu_bar(self):
        """Создание меню в верхней части окна"""
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # Меню "Файл"
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Файл", menu=file_menu)
        
        file_menu.add_command(label="Сохранить игру", command=self.save_game, accelerator="Ctrl+S")
        file_menu.add_separator()
        file_menu.add_command(label="Новая игра", command=self.new_game_from_menu)
        file_menu.add_separator()
        file_menu.add_command(label="Выйти в меню", command=self.return_to_menu)
        file_menu.add_command(label="Выйти из игры", command=self.root.quit)
        
        # Меню "Помощь"
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Помощь", menu=help_menu)
        help_menu.add_command(label="Правила игры", command=self.show_rules)
        help_menu.add_command(label="Об игре", command=self.show_about)
        
        # Горячие клавиши
        self.root.bind("<Control-s>", lambda e: self.save_game())
        self.root.bind("<Control-S>", lambda e: self.save_game())

    def load_game_on_start(self, filename):
        """Загрузка игры при старте"""
        try:
            if os.path.exists(filename):
                print(f"Загружаем игру из файла: {filename}")
                with open(filename, 'r') as f:
                    game_data = json.load(f)
                
                print("Файл успешно прочитан")
                print(f"Игроки в файле: {game_data.get('players')}")
                
                # Обновляем состояние игры
                if "players" in game_data:
                    self.players = game_data["players"]
                    print(f"Игроки загружены: {len(self.players)} игрока(ов)")
                
                if "property_owners" in game_data:
                    # Преобразуем строковые ключи в целочисленные
                    self.property_owners = {}
                    for key, value in game_data["property_owners"].items():
                        try:
                            key_int = int(key)
                            self.property_owners[key_int] = value
                        except (ValueError, TypeError):
                            self.property_owners[key] = value
                    print(f"Собственность загружена (преобразованы ключи)")
                
                if "current_player" in game_data:
                    self.current_player = game_data["current_player"]
                    print(f"Текущий игрок: {self.current_player}")
                
                if "go_passed_count" in game_data:
                    self.go_passed_count = game_data["go_passed_count"]
                    print(f"GO пройдено: {self.go_passed_count} раз")
                
                # Ensure property_owners has all 40 cells
                for i in range(40):
                    if i not in self.property_owners:
                        if self.cell_configs[i]["type"] in ["property", "railroad", "utility"]:
                            self.property_owners[i] = {
                                "owner": None, 
                                "houses": 0, 
                                "hotel": False,
                                "mortgaged": False
                            }
                        else:
                            self.property_owners[i] = None
                
                # Initial draw
                self.root.update()
                self.draw_board()
                self.draw_players()
                self.update_player_info()
                self.update_owned_streets_list()
                
                messagebox.showinfo("Игра загружена", 
                                  f"Игра успешно загружена из файла!\n"
                                  f"GO пройдено: {self.go_passed_count} раз\n"
                                  f"Сохранено: {game_data.get('timestamp', 'Неизвестно')}")
            else:
                print(f"Файл не найден: {filename}")
                messagebox.showwarning("Файл не найден", "Файл сохранения не найден. Начинаем новую игру.")
                # Initial draw
                self.root.update()
                self.draw_board()
                self.draw_players()
        except Exception as e:
            print(f"Ошибка при загрузке игры: {str(e)}")
            import traceback
            traceback.print_exc()
            messagebox.showerror("Ошибка загрузки", f"Не удалось загрузить игру:\n{str(e)}")
            # Initial draw даже при ошибке
            self.root.update()
            self.draw_board()
            self.draw_players()

    def new_game_from_menu(self):
        """Начать новую игру из меню"""
        response = messagebox.askyesno("Новая игра", 
                                      "Вы уверены, что хотите начать новую игру?\nВсе несохраненные данные будут утеряны.")
        if response:
            self.root.destroy()
            start_new_game()

    def return_to_menu(self):
        """Вернуться в главное меню"""
        response = messagebox.askyesno("Вернуться в меню", 
                                      "Вы уверены, что хотите вернуться в главное меню?\nРекомендуется сохранить игру перед выходом.")
        if response:
            self.root.destroy()
            create_start_menu()

    def show_rules(self):
        """Показать правила игры"""
        rules_text = """🎲 ПРАВИЛА ИГРЫ МОНОПОЛИЯ:

Основные правила:
1. Каждый игрок начинает с $1500 на позиции GO
2. Бросайте кубики для передвижения по полю
3. Приземляясь на свободную собственность, вы можете её купить
4. Если собственность уже куплена, вы платите аренду владельцу
5. Собирайте все свойства одного цвета для строительства домов

Специальные поля:
• 🚀 GO: Получите $200 при прохождении или приземлении
• 🚓 Jail: Посещение тюрьмы
• ⛓️ Go To Jail: Отправляйтесь в тюрьму
• ❓ Chance: Вытяните карту шанса
• 💰 Community Chest: Вытяните карту общественной казны
• 🚂 Railroad: Можно купить 4 разных железных дороги
• 💡 Utility: Коммунальные предприятия

Автосохранение:
• Игра автоматически сохраняется при каждом прохождении GO
• Игрок получает $200 за каждый пройденный круг
• Можно загрузить автосохранение из главного меню

Цель игры:
Разорить остальных игроков и не обанкротиться самому!"""
        
        messagebox.showinfo("Правила игры", rules_text)

    def show_about(self):
        """Показать информацию об игре"""
        about_text = """🏠 MONOPOLY - Классическая настольная игра

Версия: 1.0
Разработчики: CoolMonopolyGameTeam

Особенности этой версии:
• Полноценное игровое поле
• Анимация броска кубиков в центре поля
• Система сохранения/загрузки игр
• Автосохранение при прохождении GO
• Авторежим для тестирования
• Развитие собственности (дома/отели)
• Карты шанса и общественной казны
• Подробная статистика игроков

© 2025 Все права защищены"""
        
        messagebox.showinfo("Об игре", about_text)

    def create_info_panel(self, parent):
        """Creates information and control panel"""
        notebook = ttk.Notebook(parent)
        notebook.pack(fill=tk.BOTH, expand=True)
        
        # Game Info Tab
        game_frame = ttk.Frame(notebook)
        notebook.add(game_frame, text="ИНФО ИГРЫ")
        
        # Owned Streets Tab
        owned_frame = ttk.Frame(notebook)
        notebook.add(owned_frame, text="ВАША СОБСТВЕННОСТЬ")
        
        # Save/Load Tab
        save_frame = ttk.Frame(notebook)
        notebook.add(save_frame, text="СОХРАНЕНИЕ")
        
        self.create_game_info_tab(game_frame)
        self.create_owned_streets_tab(owned_frame)
        self.create_save_load_tab(save_frame)

    def create_game_info_tab(self, parent):
        """Creates game information tab"""
        # Current player info
        player_frame = ttk.LabelFrame(parent, text="ТЕКУЩИЙ ИГРОК", padding=15)
        player_frame.pack(fill=tk.X, pady=(0, 15))

        self.player_var = tk.StringVar(value=self.players[self.current_player]["name"])
        player_label = ttk.Label(player_frame, textvariable=self.player_var, 
                               font=("Arial", 14, "bold"), foreground="darkblue")
        player_label.pack()

        self.money_var = tk.StringVar(value=f"Деньги: ${self.players[self.current_player]['money']}")
        money_label = ttk.Label(player_frame, textvariable=self.money_var,
                              font=("Arial", 12, "bold"))
        money_label.pack()

        self.jail_var = tk.StringVar(value="")
        jail_label = ttk.Label(player_frame, textvariable=self.jail_var,
                             font=("Arial", 11, "bold"), foreground="red")
        jail_label.pack()

        # GO counter
        self.go_counter_var = tk.StringVar(value=f"GO пройдено: {self.go_passed_count} раз")
        go_counter_label = ttk.Label(player_frame, textvariable=self.go_counter_var,
                                   font=("Arial", 10, "italic"), foreground="darkorange")
        go_counter_label.pack(pady=(5, 0))

        # Development info
        self.dev_info_var = tk.StringVar(value="")
        dev_info_label = ttk.Label(player_frame, textvariable=self.dev_info_var,
                                 font=("Arial", 10), foreground="darkgreen")
        dev_info_label.pack(pady=(5, 0))

        # Building costs info
        self.build_costs_var = tk.StringVar(value="")
        build_costs_label = ttk.Label(player_frame, textvariable=self.build_costs_var,
                                    font=("Arial", 10), foreground="purple")
        build_costs_label.pack(pady=(5, 0))

        # Control buttons
        control_frame = ttk.LabelFrame(parent, text="УПРАВЛЕНИЕ", padding=15)
        control_frame.pack(fill=tk.X, pady=(0, 15))

        ttk.Button(control_frame, text="🎲 БРОСИТЬ КУБИКИ", 
                  command=self.roll_dice, style="Accent.TButton").pack(fill=tk.X, pady=5)
        ttk.Button(control_frame, text="🏠 КУПИТЬ СОБСТВЕННОСТЬ", 
                  command=self.buy_property).pack(fill=tk.X, pady=5)
        
        # Property development menu button
        ttk.Button(control_frame, text="🏗️ ПОСТРОИТЬ НЕДВИЖИМОСТЬ", 
                  command=self.show_property_development_menu).pack(fill=tk.X, pady=5)
        
        ttk.Button(control_frame, text="🏚️ ПРОДАТЬ НЕДВИЖИМОСТЬ", 
                  command=self.show_property_sell_menu).pack(fill=tk.X, pady=5)
        
        ttk.Button(control_frame, text="➡️ СЛЕДУЮЩИЙ ХОД", 
                  command=self.next_turn).pack(fill=tk.X, pady=5)

        ttk.Button(control_frame, text="💰 ЗАПЛАТИТЬ $50 И ВЫЙТИ ИЗ ТЮРЬМЫ", 
                  command=self.pay_jail_fine).pack(fill=tk.X, pady=5)

        # Auto mode
        auto_frame = ttk.LabelFrame(parent, text="АВТОРЕЖИМ", padding=15)
        auto_frame.pack(fill=tk.X, pady=(0, 15))

        ttk.Button(auto_frame, text="▶️ АВТОИГРА", 
                  command=self.start_auto_mode, style="Success.TButton").pack(fill=tk.X, pady=5)
        ttk.Button(auto_frame, text="⏸️ ПАУЗА", 
                  command=self.pause_auto_mode, style="Warning.TButton").pack(fill=tk.X, pady=5)
        ttk.Button(auto_frame, text="⏹️ СТОП", 
                  command=self.stop_auto_mode, style="Danger.TButton").pack(fill=tk.X, pady=5)

        # Информация о задержке
        delay_frame = ttk.Frame(auto_frame)
        delay_frame.pack(fill=tk.X, pady=(5, 0))
        
        ttk.Label(delay_frame, text="Задержка авторежима:", font=("Arial", 9)).pack(side=tk.LEFT)
        ttk.Label(delay_frame, text=f" {self.auto_delay} мс", font=("Arial", 9, "bold"), 
                 foreground="darkblue").pack(side=tk.LEFT)

        # Cell information
        self.cell_info_frame = ttk.LabelFrame(parent, text="ИНФОРМАЦИЯ О КЛЕТКЕ", padding=15)
        self.cell_info_frame.pack(fill=tk.BOTH, expand=True)

        self.cell_name_var = tk.StringVar(value="Выберите клетку")
        cell_name_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_name_var, 
                                  font=("Arial", 12, "bold"), foreground="darkred")
        cell_name_label.pack(pady=5)

        self.cell_price_var = tk.StringVar(value="")
        cell_price_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_price_var,
                                   font=("Arial", 11))
        cell_price_label.pack(pady=3)

        self.cell_owner_var = tk.StringVar(value="")
        cell_owner_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_owner_var,
                                   font=("Arial", 11))
        cell_owner_label.pack(pady=3)

        self.cell_rent_var = tk.StringVar(value="")
        cell_rent_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_rent_var,
                                  font=("Arial", 11), foreground="darkblue")
        cell_rent_label.pack(pady=3)

        self.cell_dev_var = tk.StringVar(value="")
        cell_dev_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_dev_var,
                                 font=("Arial", 11), foreground="darkgreen")
        cell_dev_label.pack(pady=3)

        self.cell_build_cost_var = tk.StringVar(value="")
        cell_build_cost_label = ttk.Label(self.cell_info_frame, textvariable=self.cell_build_cost_var,
                                        font=("Arial", 11), foreground="purple")
        cell_build_cost_label.pack(pady=3)

    def show_property_development_menu(self):
        """Show property development menu"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        if config["type"] != "property" or config["color"] not in self.color_groups:
            messagebox.showwarning("Невозможно строить", "Вы можете строить только на собственных цветных участках!")
            return
            
        prop_info = self.property_owners[position]
        if not prop_info or prop_info["owner"] != player["name"]:
            messagebox.showwarning("Невозможно строить", "Вы не владеете этим участком!")
            return
            
        # Check if player owns all properties of this color
        color_group = self.color_groups[config["color"]]
        for prop_idx in color_group["properties"]:
            prop_owner_info = self.property_owners[prop_idx]
            if not prop_owner_info or prop_owner_info["owner"] != player["name"]:
                messagebox.showwarning("Невозможно строить", 
                                     f"Сначала нужно владеть всеми {config['color']} свойствами!")
                return
        
        # Create development menu
        development_window = tk.Toplevel(self.root)
        development_window.title(f"Развитие недвижимости: {config['name']}")
        development_window.geometry("400x500")
        development_window.transient(self.root)
        development_window.grab_set()
        
        # Current state
        info_frame = ttk.LabelFrame(development_window, text="Текущее состояние", padding=15)
        info_frame.pack(fill=tk.X, padx=20, pady=10)
        
        current_state = ""
        if prop_info["hotel"]:
            current_state = "🏨 ОТЕЛЬ"
        elif prop_info["houses"] > 0:
            current_state = f"🏠 {prop_info['houses']} дом(ов)"
        else:
            current_state = "Нет строений"
            
        ttk.Label(info_frame, text=current_state, font=("Arial", 12, "bold"), 
                 foreground="darkgreen").pack()
        
        # Building costs
        costs = self.color_groups[config["color"]]
        ttk.Label(info_frame, 
                 text=f"Стоимость строительства:\n🏠 Дом: ${costs['house_cost']}\n🏨 Отель: ${costs['hotel_cost']}",
                 font=("Arial", 10)).pack(pady=5)
        
        # House building section
        house_frame = ttk.LabelFrame(development_window, text="ПОСТРОИТЬ ДОМА", padding=15)
        house_frame.pack(fill=tk.X, padx=20, pady=10)
        
        if prop_info["hotel"]:
            ttk.Label(house_frame, text="На участке уже есть отель!\nСначала продайте отель.", 
                     foreground="red").pack()
        else:
            houses_left = 4 - prop_info["houses"]
            if houses_left <= 0:
                ttk.Label(house_frame, text="Максимальное количество домов (4)!\nТеперь можно построить отель.", 
                         foreground="darkblue").pack()
            else:
                # Individual house buttons
                house_buttons_frame = ttk.Frame(house_frame)
                house_buttons_frame.pack()
                
                for i in range(1, min(houses_left, 4) + 1):
                    button_frame = ttk.Frame(house_buttons_frame)
                    button_frame.pack(pady=5, fill=tk.X)
                    
                    total_cost = costs["house_cost"] * i
                    ttk.Button(button_frame, text=f"🏠 Построить {i} дом(ов) - ${total_cost}",
                              command=lambda num=i: self.build_multiple_houses(num, development_window),
                              style="Accent.TButton").pack(fill=tk.X)
        
        # Hotel building section
        hotel_frame = ttk.LabelFrame(development_window, text="ПОСТРОИТЬ ОТЕЛЬ", padding=15)
        hotel_frame.pack(fill=tk.X, padx=20, pady=10)
        
        if prop_info["hotel"]:
            ttk.Label(hotel_frame, text="На участке уже есть отель!", 
                     foreground="red").pack()
        elif prop_info["houses"] == 4:
            ttk.Button(hotel_frame, text=f"🏨 Построить отель - ${costs['hotel_cost']}",
                      command=lambda: self.build_hotel_from_menu(development_window),
                      style="Success.TButton").pack(fill=tk.X)
        else:
            ttk.Label(hotel_frame, text=f"Для постройки отеля нужно 4 дома\nСейчас домов: {prop_info['houses']}", 
                     foreground="orange").pack()
        
        # Close button
        ttk.Button(development_window, text="ЗАКРЫТЬ", 
                  command=development_window.destroy).pack(pady=20)

    def show_property_sell_menu(self):
        """Show property sell menu"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        if config["type"] != "property" or config["color"] not in self.color_groups:
            messagebox.showwarning("Невозможно продать", "Вы можете продавать только собственные цветные участки!")
            return
            
        prop_info = self.property_owners[position]
        if not prop_info or prop_info["owner"] != player["name"]:
            messagebox.showwarning("Невозможно продать", "Вы не владеете этим участком!")
            return
            
        # Check if player owns all properties of this color
        color_group = self.color_groups[config["color"]]
        for prop_idx in color_group["properties"]:
            prop_owner_info = self.property_owners[prop_idx]
            if not prop_owner_info or prop_owner_info["owner"] != player["name"]:
                messagebox.showwarning("Невозможно продать", 
                                     f"Вы должны владеть всеми {config['color']} свойствами, чтобы продавать строения!")
                return
        
        # Create sell menu
        sell_window = tk.Toplevel(self.root)
        sell_window.title(f"Продажа недвижимости: {config['name']}")
        sell_window.geometry("400x400")
        sell_window.transient(self.root)
        sell_window.grab_set()
        
        # Current state
        info_frame = ttk.LabelFrame(sell_window, text="Текущее состояние", padding=15)
        info_frame.pack(fill=tk.X, padx=20, pady=10)
        
        current_state = ""
        if prop_info["hotel"]:
            current_state = "🏨 ОТЕЛЬ"
        elif prop_info["houses"] > 0:
            current_state = f"🏠 {prop_info['houses']} дом(ов)"
        else:
            current_state = "Нет строений"
            
        ttk.Label(info_frame, text=current_state, font=("Arial", 12, "bold"), 
                 foreground="darkgreen").pack()
        
        # Sell prices
        costs = self.color_groups[config["color"]]
        ttk.Label(info_frame, 
                 text=f"Цена продажи (50% от стоимости):\n🏠 Дом: ${costs['house_cost'] // 2}\n🏨 Отель: ${costs['hotel_cost'] // 2}",
                 font=("Arial", 10)).pack(pady=5)
        
        # House selling section
        if prop_info["houses"] > 0 and not prop_info["hotel"]:
            house_frame = ttk.LabelFrame(sell_window, text="ПРОДАТЬ ДОМА", padding=15)
            house_frame.pack(fill=tk.X, padx=20, pady=10)
            
            house_buttons_frame = ttk.Frame(house_frame)
            house_buttons_frame.pack()
            
            # Можно продать только по одному дому за раз
            ttk.Button(house_buttons_frame, text=f"🏚️ Продать 1 дом - ${costs['house_cost'] // 2}",
                      command=lambda: self.sell_house_from_menu(sell_window),
                      style="Warning.TButton").pack(fill=tk.X, pady=5)
        
        # Hotel selling section
        if prop_info["hotel"]:
            hotel_frame = ttk.LabelFrame(sell_window, text="ПРОДАТЬ ОТЕЛЬ", padding=15)
            hotel_frame.pack(fill=tk.X, padx=20, pady=10)
            
            ttk.Button(hotel_frame, text=f"💵 Продать отель - ${costs['hotel_cost'] // 2}",
                      command=lambda: self.sell_hotel_from_menu(sell_window),
                      style="Danger.TButton").pack(fill=tk.X)
        
        # Close button
        ttk.Button(sell_window, text="ЗАКРЫТЬ", 
                  command=sell_window.destroy).pack(pady=20)

    def build_hotel_from_menu(self, window):
        """Build hotel from menu"""
        self.build_hotel()
        window.destroy()

    def sell_house_from_menu(self, window):
        """Sell house from menu"""
        self.sell_house()
        window.destroy()

    def sell_hotel_from_menu(self, window):
        """Sell hotel from menu"""
        self.sell_hotel()
        window.destroy()

    def create_owned_streets_tab(self, parent):
        """Создает вкладку с прокручиваемым списком собственности"""
        main_frame = ttk.Frame(parent)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Создаем Canvas для прокрутки
        self.owned_canvas = tk.Canvas(main_frame, bg="white", highlightthickness=0)
        scrollbar = ttk.Scrollbar(main_frame, orient="vertical", command=self.owned_canvas.yview)
        
        # Создаем фрейм внутри Canvas для содержимого
        self.owned_scrollable_frame = ttk.Frame(self.owned_canvas)
        
        # Создаем окно в Canvas для нашего фрейма
        self.canvas_window = self.owned_canvas.create_window((0, 0), window=self.owned_scrollable_frame, anchor="nw")
        
        # Настраиваем прокрутку
        self.owned_canvas.configure(yscrollcommand=scrollbar.set)
        
        # Упаковываем элементы
        self.owned_canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        
        # Настраиваем обновление области прокрутки
        def configure_scroll_region(event):
            # Обновляем область прокрутки
            self.owned_canvas.configure(scrollregion=self.owned_canvas.bbox("all"))
            # Устанавливаем ширину окна равной ширине Canvas
            self.owned_canvas.itemconfig(self.canvas_window, width=self.owned_canvas.winfo_width())
        
        self.owned_scrollable_frame.bind("<Configure>", configure_scroll_region)
        
        # Настраиваем прокрутку колесиком мыши
        def on_mouse_wheel(event):
            # Прокрутка вверх/вниз
            if event.delta:
                self.owned_canvas.yview_scroll(-1 * int(event.delta / 120), "units")
            else:
                # Для Linux/Mac
                if event.num == 4:
                    self.owned_canvas.yview_scroll(-1, "units")
                elif event.num == 5:
                    self.owned_canvas.yview_scroll(1, "units")
            return "break"
        
        # Привязываем события колесика мыши
        self.owned_canvas.bind_all("<MouseWheel>", on_mouse_wheel)  # Windows
        self.owned_canvas.bind_all("<Button-4>", on_mouse_wheel)    # Linux (вверх)
        self.owned_canvas.bind_all("<Button-5>", on_mouse_wheel)    # Linux (вниз)
        
        # Привязываем событие изменения размера
        self.owned_canvas.bind("<Configure>", lambda e: self.owned_canvas.itemconfig(self.canvas_window, width=e.width))
        
        # Кнопка обновления
        button_frame = ttk.Frame(parent)
        button_frame.pack(fill=tk.X, padx=5, pady=5)
        
        update_button = ttk.Button(button_frame, text="🔄 ОБНОВИТЬ СПИСОК", 
                                 command=self.update_owned_streets_list)
        update_button.pack(pady=5)
        
        # Информация о том, как прокручивать
        info_label = ttk.Label(button_frame, 
                              text="💡 Используйте колесико мыши для прокрутки списка",
                              font=("Arial", 8, "italic"),
                              foreground="gray")
        info_label.pack(pady=2)
        
        # Обновляем список при создании
        self.update_owned_streets_list()

    def create_save_load_tab(self, parent):
        """Creates save/load tab"""
        save_frame = ttk.LabelFrame(parent, text="СОХРАНИТЬ ИГРУ", padding=20)
        save_frame.pack(fill=tk.X, pady=(0, 20), padx=5)
        
        # Только одна кнопка сохранения
        ttk.Button(save_frame, text="💾 СОХРАНИТЬ ИГРУ", 
                  command=self.save_game, style="Accent.TButton").pack(fill=tk.X, pady=10)
        
        save_info_frame = ttk.Frame(save_frame)
        save_info_frame.pack(fill=tk.X, pady=10)
        
        self.save_status_var = tk.StringVar(value="Готово к сохранению")
        save_status_label = ttk.Label(save_info_frame, textvariable=self.save_status_var,
                                    font=("Arial", 10), foreground="darkblue")
        save_status_label.pack()
        
        load_frame = ttk.LabelFrame(parent, text="ЗАГРУЗИТЬ ИГРУ", padding=20)
        load_frame.pack(fill=tk.X, pady=(0, 20), padx=5)
        
        # Кнопка для загрузки автосохранения
        autosave_button = ttk.Button(load_frame, text="📀 ЗАГРУЗИТЬ АВТОСОХРАНЕНИЕ", 
                                   command=lambda: self.load_game("autosave.monopoly"),
                                   style="Success.TButton")
        autosave_button.pack(fill=tk.X, pady=10)
        
        load_info_frame = ttk.Frame(load_frame)
        load_info_frame.pack(fill=tk.X, pady=10)
        
        self.load_status_var = tk.StringVar(value="Готово к загрузке")
        load_status_label = ttk.Label(load_info_frame, textvariable=self.load_status_var,
                                    font=("Arial", 10), foreground="darkgreen")
        load_status_label.pack()
        
        # Info section
        info_frame = ttk.LabelFrame(parent, text="ИНФОРМАЦИЯ О СОХРАНЕНИЕ", padding=15)
        info_frame.pack(fill=tk.BOTH, expand=True, padx=5)
        
        info_text = """💡 СИСТЕМА СОХРАНЕНИЯ:

• Сохраняются все данные игры:
  - Позиции и деньги игроков
  - Владение собственностью
  - Дома и отели
  - Текущий ход
  - Статус тюрьмы
  - Счетчик прохождений GO

• АВТОСОХРАНЕНИЕ:
  - Создается при каждом прохождении клетки GO
  - При каждом сохранении игры
  - Файл: autosave.monopoly

• Файлы сохраняются в формате JSON
• Расширение файлов: .monopoly

⚠️ ВНИМАНИЕ: Загрузка игры перезаписывает
   текущую игровую сессию!"""
        
        info_label = ttk.Label(info_frame, text=info_text, font=("Arial", 10),
                              justify="left", background="#f0f0f0", padding=10)
        info_label.pack(fill=tk.BOTH, expand=True)

    def save_game(self, filename=None, autosave=False):
        """Save game to file"""
        try:
            # Подготавливаем данные для сохранения с исправленной позицией игрока
            game_data = {
                "version": "1.0",
                "current_player": self.current_player,
                "players": self.players.copy(),
                "property_owners": self.property_owners,
                "go_passed_count": self.go_passed_count,
                "timestamp": time.strftime("%Y-%m-%d %H:%M:%S")
            }
            
            if not filename and not autosave:
                # Ask for file location
                filename = filedialog.asksaveasfilename(
                    defaultextension=".monopoly",
                    filetypes=[
                        ("Monopoly Game Files", "*.monopoly"),
                        ("JSON Files", "*.json"),
                        ("All Files", "*.*")
                    ],
                    title="Сохранить игру"
                )
            
            if filename or autosave:
                if filename:
                    # Save to selected file
                    with open(filename, 'w') as f:
                        json.dump(game_data, f, indent=4)
                
                # Always create/update autosave
                with open("autosave.monopoly", 'w') as f:
                    json.dump(game_data, f, indent=4)
                
                if not autosave:
                    self.save_status_var.set(f"Игра сохранена: {os.path.basename(filename)}")
                    messagebox.showinfo("Сохранено", f"Игра успешно сохранена!\nФайл: {filename}")
                else:
                    self.save_status_var.set(f"Автосохранение создано (GO пройдено: {self.go_passed_count})")
                
                return True
            else:
                self.save_status_var.set("Сохранение отменено")
                return False
                
        except Exception as e:
            self.save_status_var.set(f"Ошибка сохранения: {str(e)}")
            if not autosave:
                messagebox.showerror("Ошибка", f"Не удалось сохранить игру:\n{str(e)}")
            return False

    def load_game(self, filename=None):
        """Load game from file"""
        try:
            if not filename:
                # Ask for file location
                filename = filedialog.askopenfilename(
                    defaultextension=".monopoly",
                    filetypes=[
                        ("Monopoly Game Files", "*.monopoly"),
                        ("JSON Files", "*.json"),
                        ("All Files", "*.*")
                    ],
                    title="Загрузить игру"
                )
            
            if not filename:
                self.load_status_var.set("Загрузка отменена")
                return False
            
            # Check if file exists
            if not os.path.exists(filename):
                self.load_status_var.set("Файл не найден")
                messagebox.showerror("Ошибка", "Файл не найден!")
                return False
            
            # Load from file
            print(f"Загружаем игру из файла: {filename}")
            with open(filename, 'r') as f:
                game_data = json.load(f)
            
            print("Файл успешно прочитан")
            
            # Validate game data
            required_keys = ["players", "property_owners", "current_player"]
            if not all(key in game_data for key in required_keys):
                self.load_status_var.set("Некорректный файл игры")
                messagebox.showerror("Ошибка", "Некорректный формат файла игры!")
                return False
            
            # Update game state
            self.players = game_data["players"]
            
            # Преобразуем строковые ключи в целочисленные при загрузке
            self.property_owners = {}
            for key, value in game_data["property_owners"].items():
                try:
                    key_int = int(key)
                    self.property_owners[key_int] = value
                except (ValueError, TypeError):
                    self.property_owners[key] = value
            
            self.current_player = game_data["current_player"]
            
            if "go_passed_count" in game_data:
                self.go_passed_count = game_data["go_passed_count"]
            else:
                self.go_passed_count = 0
            
            # Ensure property_owners has all 40 cells
            for i in range(40):
                if i not in self.property_owners:
                    if self.cell_configs[i]["type"] in ["property", "railroad", "utility"]:
                        self.property_owners[i] = {
                            "owner": None, 
                            "houses": 0, 
                            "hotel": False,
                            "mortgaged": False
                        }
                    else:
                        self.property_owners[i] = None
            
            # Update UI
            self.update_player_info()
            self.draw_board()
            self.draw_players()
            self.update_owned_streets_list()
            
            self.load_status_var.set(f"Игра загружена: {os.path.basename(filename)}")
            messagebox.showinfo("Загружено", 
                              f"Игра успешно загружена!\n"
                              f"GO пройдено: {self.go_passed_count} раз\n"
                              f"Сохранено: {game_data.get('timestamp', 'Неизвестно')}\n"
                              f"Сейчас ходит: {self.players[self.current_player]['name']}")
            
            return True
            
        except json.JSONDecodeError:
            self.load_status_var.set("Некорректный JSON файл")
            messagebox.showerror("Ошибка", "Некорректный формат JSON файла!")
            return False
        except Exception as e:
            self.load_status_var.set(f"Ошибка загрузки: {str(e)}")
            messagebox.showerror("Ошибка", f"Не удалось загрузить игру:\n{str(e)}")
            return False

    def update_owned_streets_list(self):
        """Обновляет список собственности в прокручиваемой области"""
        # Очищаем текущее содержимое
        for widget in self.owned_scrollable_frame.winfo_children():
            widget.destroy()
        
        any_properties = False
        
        for player in self.players:
            # Ищем собственность, принадлежащую игроку
            owned_properties = []
            for prop_idx, info in self.property_owners.items():
                if info and isinstance(info, dict) and info.get("owner") == player["name"]:
                    owned_properties.append(prop_idx)
            
            if owned_properties:
                any_properties = True
                
                player_section = ttk.LabelFrame(self.owned_scrollable_frame, 
                                              text=f"🎮 {player['name']} - Деньги: ${player['money']}",
                                              padding=10)
                player_section.pack(fill=tk.X, pady=10, padx=5)
                
                total_properties = len(owned_properties)
                properties_label = ttk.Label(player_section, 
                                           text=f"Всего собственности: {total_properties}",
                                           font=("Arial", 11, "bold"))
                properties_label.pack(anchor="w", pady=(0, 10))
                
                total_houses = 0
                total_hotels = 0
                total_investment = 0
                
                for prop_idx in owned_properties:
                    prop_info = self.property_owners.get(prop_idx)
                    if not prop_info:
                        continue
                    
                    # Получаем конфигурацию клетки
                    config = self.cell_configs.get(prop_idx)
                    if not config:
                        continue
                    
                    if prop_info.get("hotel"):
                        total_hotels += 1
                        if config["color"] in self.color_groups:
                            total_investment += self.color_groups[config["color"]]["hotel_cost"]
                            total_investment += 4 * self.color_groups[config["color"]]["house_cost"]
                    else:
                        total_houses += prop_info.get("houses", 0)
                        if config["color"] in self.color_groups:
                            total_investment += prop_info.get("houses", 0) * self.color_groups[config["color"]]["house_cost"]
                
                if total_houses > 0 or total_hotels > 0:
                    dev_label = ttk.Label(player_section,
                                        text=f"Развитие: 🏠 {total_houses} домов | 🏨 {total_hotels} отелей | Общие инвестиции: ${total_investment}",
                                        font=("Arial", 10),
                                        foreground="darkgreen")
                    dev_label.pack(anchor="w", pady=(0, 10))
                
                properties_frame = ttk.Frame(player_section)
                properties_frame.pack(fill=tk.X, padx=5)
                
                for prop_idx in owned_properties:
                    prop_info = self.property_owners.get(prop_idx)
                    if not prop_info:
                        continue
                    
                    # Получаем конфигурацию клетки
                    config = self.cell_configs.get(prop_idx)
                    if not config:
                        continue
                    
                    prop_frame = ttk.Frame(properties_frame, relief="solid", borderwidth=1)
                    prop_frame.pack(fill=tk.X, pady=3, padx=2)
                    
                    # Цветная полоска слева
                    color_frame = ttk.Frame(prop_frame, width=5)
                    color_frame.pack(side="left", fill="y", padx=(0, 5))
                    
                    # Устанавливаем цвет в зависимости от группы
                    color_name = config.get("color", "")
                    if color_name in self.cell_colors:
                        color_hex = self.cell_colors[color_name]
                        # Создаем Canvas для цветной полоски
                        color_canvas = tk.Canvas(color_frame, width=5, height=60, 
                                               bg=color_hex, highlightthickness=0)
                        color_canvas.pack(fill="both", expand=True)
                    
                    content_frame = ttk.Frame(prop_frame)
                    content_frame.pack(side="left", fill="both", expand=True, padx=5)
                    
                    name_label = ttk.Label(content_frame, 
                                         text=f"🏠 {config.get('name', 'Неизвестно')}",
                                         font=("Arial", 10, "bold"),
                                         anchor="w")
                    name_label.pack(fill=tk.X, padx=5, pady=2)
                    
                    info_label = ttk.Label(content_frame,
                                         text=f"Цена: ${config.get('price', 0)} | Цвет: {config.get('color', '').replace('_', ' ').title()}",
                                         font=("Arial", 9),
                                         foreground="gray",
                                         anchor="w")
                    info_label.pack(fill=tk.X, padx=5, pady=1)
                    
                    if config.get("color") in self.color_groups:
                        costs = self.color_groups[config["color"]]
                        costs_label = ttk.Label(content_frame,
                                              text=f"Стоимость строительства: 🏠 ${costs['house_cost']} каждый | 🏨 ${costs['hotel_cost']}",
                                              font=("Arial", 8),
                                              foreground="purple",
                                              anchor="w")
                        costs_label.pack(fill=tk.X, padx=5, pady=1)
                    
                    dev_status = ""
                    rent_info = ""
                    
                    if prop_info.get("hotel"):
                        dev_status = "🏨 ОТЕЛЬ"
                        if prop_idx in self.rent_tables:
                            current_rent = self.rent_tables[prop_idx][5]
                            rent_info = f"Текущая аренда с отелем: ${current_rent}"
                    elif prop_info.get("houses", 0) > 0:
                        dev_status = f"🏠 {prop_info.get('houses', 0)} дом(ов)"
                        if prop_idx in self.rent_tables:
                            current_rent = self.rent_tables[prop_idx][prop_info.get("houses", 0)]
                            rent_info = f"Текущая аренда: ${current_rent}"
                    else:
                        dev_status = "Нет домов"
                        if prop_idx in self.rent_tables:
                            current_rent = self.rent_tables[prop_idx][0]
                            rent_info = f"Базовая аренда: ${current_rent}"
                    
                    if dev_status:
                        dev_label = ttk.Label(content_frame,
                                            text=dev_status,
                                            font=("Arial", 9, "bold"),
                                            foreground="darkgreen",
                                            anchor="w")
                        dev_label.pack(fill=tk.X, padx=5, pady=1)
                    
                    if rent_info:
                        rent_label = ttk.Label(content_frame,
                                             text=rent_info,
                                             font=("Arial", 8),
                                             foreground="darkblue",
                                             anchor="w")
                        rent_label.pack(fill=tk.X, padx=5, pady=1)
        
        if not any_properties:
            no_props_frame = ttk.Frame(self.owned_scrollable_frame)
            no_props_frame.pack(fill=tk.BOTH, expand=True, pady=50)
            
            no_props_label = ttk.Label(no_props_frame, 
                                      text="🎮 Собственность еще не куплена!\n\nКупите собственность, чтобы увидеть ее здесь.",
                                      font=("Arial", 12, "italic"),
                                      foreground="gray",
                                      justify="center")
            no_props_label.pack()
        
        # Обновляем область прокрутки
        self.owned_scrollable_frame.update_idletasks()
        self.owned_canvas.configure(scrollregion=self.owned_canvas.bbox("all"))

    def start_auto_mode(self):
        """Start auto mode"""
        self.auto_mode = True
        self.auto_paused = False
        messagebox.showinfo("Авторежим", f"Авторежим запущен!\nЗадержка: {self.auto_delay}мс\nАвтоматически будут покупаться свободные участки и строиться дома.")
        self.auto_play()

    def pause_auto_mode(self):
        """Pause auto mode"""
        self.auto_paused = not self.auto_paused
        status = "приостановлен" if self.auto_paused else "возобновлен"
        messagebox.showinfo("Авторежим", f"Авторежим {status}!")

    def stop_auto_mode(self):
        """Stop auto mode"""
        self.auto_mode = False
        self.auto_paused = False
        messagebox.showinfo("Авторежим", "Авторежим остановлен!")

    def auto_play(self):
        """Auto play with delays"""
        if not self.auto_mode or self.auto_paused:
            return
        
        player = self.players[self.current_player]
        
        if player["in_jail"] and player["money"] >= 50:
            self.root.after(self.auto_delay // 2, self.auto_pay_jail_fine)
            self.root.after(self.auto_delay, self.auto_play_turn)
        else:
            self.root.after(self.auto_delay // 2, self.auto_play_turn)

    def auto_play_turn(self):
        """Execute one auto turn"""
        if not self.auto_mode or self.auto_paused:
            return
        
        self.auto_roll_dice()

    def auto_roll_dice(self):
        """Auto roll dice"""
        if not self.auto_mode or self.auto_paused:
            return
        
        current_player = self.players[self.current_player]
        
        if current_player["in_jail"]:
            self.root.after(self.auto_delay, self.auto_next_turn)
            return
            
        dice1 = random.randint(1, 6)
        dice2 = random.randint(1, 6)
        total = dice1 + dice2
        
        # Используем анимацию в авторежиме
        def after_animation():
            self.move_player(total)
            self.root.after(500, self.auto_check_purchase)  # Сразу после хода проверяем покупку
        
        self.dice_animation.roll(dice1, dice2, after_animation)

    def auto_check_purchase(self):
        """Check if we can buy property in auto mode"""
        if not self.auto_mode or self.auto_paused:
            return
        
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        # Автоматически покупаем свободную собственность, если можем
        if config["type"] in ["property", "railroad", "utility"]:
            prop_info = self.property_owners[position]
            if prop_info["owner"] is None and player["money"] >= config["price"]:
                # В авторежиме всегда покупаем доступную собственность
                self.buy_property()
                messagebox.showinfo("Автопокупка", f"{player['name']} покупает {config['name']} за ${config['price']}!")
                self.root.after(500, self.auto_check_development)
            else:
                self.root.after(500, self.auto_check_development)
        else:
            self.root.after(500, self.auto_check_development)

    def auto_check_development(self):
        """Check if we can build in auto mode"""
        if not self.auto_mode or self.auto_paused:
            return
        
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        # В авторежиме пытаемся строить на своей собственности
        if config["type"] == "property" and config["color"] in self.color_groups:
            prop_info = self.property_owners[position]
            if prop_info and prop_info["owner"] == player["name"]:
                can_build, _ = self.can_build_on_property(position)
                if can_build:
                    color = config["color"]
                    house_cost = self.color_groups[color]["house_cost"]
                    hotel_cost = self.color_groups[color]["hotel_cost"]
                    
                    # Строим отель, если можно
                    if prop_info["houses"] == 4 and not prop_info["hotel"] and player["money"] >= hotel_cost:
                        self.build_hotel()
                        messagebox.showinfo("Автостроительство", f"{player['name']} строит отель на {config['name']}!")
                    # Строим дома, если можно
                    elif prop_info["houses"] < 4 and player["money"] >= house_cost:
                        # Строим сразу максимально возможное количество домов
                        houses_to_build = min(4 - prop_info["houses"], 3)
                        if houses_to_build > 0 and player["money"] >= house_cost * houses_to_build:
                            self.build_multiple_houses(houses_to_build)
                            messagebox.showinfo("Автостроительство", 
                                              f"{player['name']} строит {houses_to_build} дом(ов) на {config['name']}!")
        
        self.root.after(500, self.auto_check_other_properties)

    def auto_check_other_properties(self):
        """Check other properties for development in auto mode"""
        if not self.auto_mode or self.auto_paused:
            return
        
        player = self.players[self.current_player]
        
        # Проверяем все свойства игрока на возможность строительства
        for prop_idx, prop_info in self.property_owners.items():
            if prop_info and isinstance(prop_info, dict) and prop_info["owner"] == player["name"]:
                config = self.cell_configs.get(prop_idx)
                if config and config["type"] == "property" and config["color"] in self.color_groups:
                    can_build, _ = self.can_build_on_property(prop_idx)
                    if can_build:
                        color = config["color"]
                        house_cost = self.color_groups[color]["house_cost"]
                        hotel_cost = self.color_groups[color]["hotel_cost"]
                        
                        # Строим отель, если можно
                        if prop_info["houses"] == 4 and not prop_info["hotel"] and player["money"] >= hotel_cost:
                            # Перемещаем игрока на эту клетку для постройки
                            old_position = player["position"]
                            player["position"] = prop_idx
                            self.build_hotel()
                            player["position"] = old_position
                            self.draw_players()
                            messagebox.showinfo("Автостроительство", 
                                              f"{player['name']} строит отель на {config['name']}!")
                        # Строим дома, если можно
                        elif prop_info["houses"] < 4 and player["money"] >= house_cost:
                            # Строим сразу несколько домов
                            houses_to_build = min(4 - prop_info["houses"], 2)
                            if houses_to_build > 0 and player["money"] >= house_cost * houses_to_build:
                                old_position = player["position"]
                                player["position"] = prop_idx
                                self.build_multiple_houses(houses_to_build)
                                player["position"] = old_position
                                self.draw_players()
                                messagebox.showinfo("Автостроительство", 
                                                  f"{player['name']} строит {houses_to_build} дом(ов) на {config['name']}!")
        
        self.root.after(500, self.auto_next_turn)

    def auto_pay_jail_fine(self):
        """Auto pay jail fine"""
        if not self.auto_mode or self.auto_paused:
            return
        
        player = self.players[self.current_player]
        if player["in_jail"] and player["money"] >= 50:
            player["money"] -= 50
            player["in_jail"] = False
            player["jail_turns"] = 0
            self.update_player_info()
            messagebox.showinfo("Авто-тюрьма", f"{player['name']} платит $50, чтобы выйти из тюрьмы!")

    def auto_next_turn(self):
        """Auto next turn"""
        if not self.auto_mode or self.auto_paused:
            return
        
        self.current_player = (self.current_player + 1) % len(self.players)
        self.update_player_info()
        self.draw_players()
        
        messagebox.showinfo("Автоход", f"Сейчас ходит {self.players[self.current_player]['name']}")
        self.root.after(self.auto_delay, self.auto_play)

    def load_icons(self):
        """Load icons"""
        icon_configs = {
            "GO": "🚀", "Jail": "🚓", "Chance": "❓", "Community Chest": "💰",
            "Free Parking": "🅿️", "Go To Jail": "⛓️", 
            "railroad": "🚂", "utility": "💡"
        }
        for name, symbol in icon_configs.items():
            self.icons[name] = symbol

    def on_resize(self, event):
        """Redraw board on resize"""
        self.board_size = min(event.width - 20, event.height - 20)
        self.cell_size = self.board_size // 11
        self.draw_board()
        self.draw_players()

    def draw_board(self):
        """Draw game board"""
        self.canvas.delete("all")
        self.cells = []

        self.canvas.create_rectangle(10, 10, self.board_size-10, self.board_size-10, 
                                   outline="black", width=4, fill="#F5F5DC")
        self.draw_center_area()

        for cell_idx in range(40):
            coords = self.calculate_cell_coordinates(cell_idx)
            if coords:
                self.draw_cell(cell_idx, *coords)
        
        self.draw_players()

    def draw_center_area(self):
        """Draw center area"""
        center_x = self.board_size // 2
        center_y = self.board_size // 2
        center_size = self.board_size // 3

        self.canvas.create_rectangle(center_x - center_size//2, center_y - center_size//2,
                                   center_x + center_size//2, center_y + center_size//2,
                                   fill="#E8E8E8", outline="black", width=3)

        self.canvas.create_text(center_x, center_y - center_size//3,
                              text="MONOPOLY", font=("Arial", 24, "bold"), fill="darkred")

        self.canvas.create_text(center_x, center_y - center_size//6,
                              text="🎲🏠💰", font=("Arial", 36), fill="darkblue")

        current_player = self.players[self.current_player]
        player_info = f"{current_player['name']}\n${current_player['money']}"
        if current_player["in_jail"]:
            player_info += f"\nВ ТЮРЬМЕ"
        
        self.canvas.create_text(center_x, center_y + center_size//4,
                              text=player_info, font=("Arial", 14, "bold"),
                              fill="darkgreen", justify="center")
        
        # Добавляем информацию о GO
        go_info = f"GO пройдено: {self.go_passed_count} раз"
        self.canvas.create_text(center_x, center_y + center_size//3 + 20,
                              text=go_info, font=("Arial", 10, "italic"),
                              fill="darkorange", justify="center")
        
        # Инициализация анимации кубиков
        self.dice_animation = DiceAnimation(self.canvas, center_x, center_y - center_size//6 + 40)

    def calculate_cell_coordinates(self, cell_idx):
        """Calculate cell coordinates"""
        board_margin = 10
        inner_size = self.board_size - 2 * board_margin
        side_length = inner_size // 10
        corner_size = side_length * 1.5
        
        if cell_idx == 0:  # GO
            return (board_margin, board_margin + inner_size - corner_size,
                    board_margin + corner_size, board_margin + inner_size)
        elif 1 <= cell_idx <= 9:
            y1 = board_margin + corner_size + (9 - cell_idx) * ((inner_size - 2 * corner_size) // 9)
            return (board_margin, y1,
                    board_margin + side_length, y1 + ((inner_size - 2 * corner_size) // 9))
        elif cell_idx == 10:  # Jail
            return (board_margin, board_margin,
                    board_margin + corner_size, board_margin + corner_size)
        elif 11 <= cell_idx <= 19:
            x1 = board_margin + corner_size + (cell_idx - 11) * ((inner_size - 2 * corner_size) // 9)
            return (x1, board_margin,
                    x1 + ((inner_size - 2 * corner_size) // 9), board_margin + side_length)
        elif cell_idx == 20:  # Free Parking
            return (board_margin + inner_size - corner_size, board_margin,
                    board_margin + inner_size, board_margin + corner_size)
        elif 21 <= cell_idx <= 29:
            y1 = board_margin + corner_size + (cell_idx - 21) * ((inner_size - 2 * corner_size) // 9)
            return (board_margin + inner_size - side_length, y1,
                    board_margin + inner_size, y1 + ((inner_size - 2 * corner_size) // 9))
        elif cell_idx == 30:  # Go To Jail
            return (board_margin + inner_size - corner_size, board_margin + inner_size - corner_size,
                    board_margin + inner_size, board_margin + inner_size)
        elif 31 <= cell_idx <= 39:
            x1 = board_margin + corner_size + (39 - cell_idx) * ((inner_size - 2 * corner_size) // 9)
            return (x1, board_margin + inner_size - side_length,
                    x1 + ((inner_size - 2 * corner_size) // 9), board_margin + inner_size)
        return None

    def draw_cell(self, cell_idx, x1, y1, x2, y2):
        """Draw individual cell"""
        config = self.cell_configs[cell_idx]
        color = self.cell_colors[config["color"]]
        
        is_horizontal = (11 <= cell_idx <= 19) or (31 <= cell_idx <= 39)
        is_vertical = (1 <= cell_idx <= 9) or (21 <= cell_idx <= 29)
        is_corner = cell_idx in [0, 10, 20, 30]
        
        cell_width = x2 - x1
        cell_height = y2 - y1
        
        cell_id = self.canvas.create_rectangle(x1, y1, x2, y2, 
                                             fill=color, outline="black", width=2)
        
        cell_name = config["name"]
        
        if is_corner:
            lines = self.wrap_text(cell_name, 10)
            text_y = y1 + 20
            for line in lines:
                self.canvas.create_text(x1 + cell_width // 2, text_y, text=line, 
                                      font=("Arial", 9, "bold"), width=cell_width - 10, 
                                      justify="center", fill="black")
                text_y += 14
            
            if config["name"] in self.icons:
                self.canvas.create_text(x1 + cell_width // 2, y1 + cell_height // 2 + 10,
                                      text=self.icons[config["name"]], font=("Arial", 20),
                                      fill="black")
                
        elif is_horizontal:
            lines = self.wrap_text(cell_name, 8)
            text_y = y1 + 15
            for line in lines:
                self.canvas.create_text(x1 + cell_width // 2, text_y, text=line,
                                      font=("Arial", 7, "bold"), width=cell_width - 6,
                                      justify="center", fill="black" if config["color"] != "dark_blue" else "white")
                text_y += 10
        else:
            lines = self.wrap_text(cell_name, 5)
            text_x = x1 + 12
            for line in lines:
                self.canvas.create_text(text_x, y1 + cell_height // 2, text=line,
                                      font=("Arial", 7, "bold"), width=cell_height - 6,
                                      justify="center", fill="black" if config["color"] != "dark_blue" else "white",
                                      angle=90)
                text_x += 10
        
        if config["price"] > 0:
            price_color = "darkred" if config["color"] != "dark_blue" else "white"
            if is_horizontal:
                self.canvas.create_text(x1 + cell_width // 2, y2 - 10,
                                      text=f"${config['price']}", font=("Arial", 8, "bold"),
                                      fill=price_color)
            elif is_vertical:
                self.canvas.create_text(x2 - 10, y1 + cell_height // 2,
                                      text=f"${config['price']}", font=("Arial", 8, "bold"),
                                      fill=price_color, angle=90)
            else:
                self.canvas.create_text(x1 + cell_width // 2, y2 - 15,
                                      text=f"${config['price']}", font=("Arial", 9, "bold"),
                                      fill=price_color)
        
        if cell_idx in self.property_owners and self.property_owners[cell_idx] and self.property_owners[cell_idx]["owner"]:
            prop_info = self.property_owners[cell_idx]
            if prop_info["houses"] > 0 or prop_info["hotel"]:
                if prop_info["hotel"]:
                    self.canvas.create_text(x1 + cell_width // 2, y1 + 25,
                                          text="🏨", font=("Arial", 16), fill="black")
                elif prop_info["houses"] > 0:
                    house_text = "🏠" * min(prop_info["houses"], 4)
                    self.canvas.create_text(x1 + cell_width // 2, y1 + 25,
                                          text=house_text, font=("Arial", 12), fill="black")
            
        if not is_corner and (config["name"] in self.icons or config["type"] in ["railroad", "utility"]):
            icon_text = self.icons.get(config["name"]) or self.icons.get(config["type"])
            if icon_text:
                icon_color = "black" if config["color"] != "dark_blue" else "white"
                if is_horizontal:
                    self.canvas.create_text(x1 + cell_width // 2, y1 + cell_height // 2 + 5,
                                          text=icon_text, font=("Arial", 12), fill=icon_color)
                else:
                    self.canvas.create_text(x1 + cell_width // 2, y1 + cell_height // 2,
                                          text=icon_text, font=("Arial", 12), fill=icon_color)

        self.cells.append({"id": cell_id, "coords": (x1, y1, x2, y2), 
                          "config": config, "index": cell_idx})
        self.canvas.tag_bind(cell_id, "<Button-1>", lambda e, idx=cell_idx: self.on_cell_click(idx))

    def wrap_text(self, text, max_length):
        """Wrap text into lines"""
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
            
        return lines if lines else [text]

    def draw_players(self):
        """Draw player tokens"""
        self.canvas.delete("player_token")
        self.player_tokens = {}
        
        for i, player in enumerate(self.players):
            cell_idx = player["position"]
            if 0 <= cell_idx < 40 and self.cells and len(self.cells) > cell_idx:
                try:
                    x1, y1, x2, y2 = self.cells[cell_idx]["coords"]
                    cell_width = x2 - x1
                    cell_height = y2 - y1
                    
                    if i == 0:
                        token_x = x1 + cell_width // 4
                        token_y = y1 + cell_height // 4
                    elif i == 1:
                        token_x = x1 + 3 * cell_width // 4
                        token_y = y1 + cell_height // 4
                    elif i == 2:
                        token_x = x1 + cell_width // 4
                        token_y = y1 + 3 * cell_height // 4
                    else:
                        token_x = x1 + 3 * cell_width // 4
                        token_y = y1 + 3 * cell_height // 4
                    
                    token_id = self.canvas.create_oval(token_x - 8, token_y - 8,
                                                     token_x + 8, token_y + 8,
                                                     fill=player["color"], outline="black", width=2,
                                                     tags="player_token")
                    
                    self.canvas.create_text(token_x, token_y, text=str(i+1),
                                          font=("Arial", 9, "bold"), fill="white", tags="player_token")
                    
                    self.player_tokens[player["name"]] = token_id
                except (IndexError, KeyError):
                    continue

    def on_cell_click(self, cell_idx):
        """Handle cell click"""
        if cell_idx < len(self.cells):
            cell = self.cells[cell_idx]
            config = cell["config"]
            self.cell_name_var.set(config["name"])
            
            if config["price"] > 0:
                self.cell_price_var.set(f"Цена покупки: ${config['price']}")
            else:
                self.cell_price_var.set("")
            
            if cell_idx in self.property_owners:
                prop_info = self.property_owners[cell_idx]
                if prop_info and prop_info["owner"]:
                    owner_name = prop_info["owner"]
                    dev_info = ""
                    if prop_info.get("hotel", False):
                        dev_info = "Отель 🏨"
                    elif prop_info.get("houses", 0) > 0:
                        dev_info = f"{prop_info['houses']} дом(ов) 🏠"
                    self.cell_owner_var.set(f"Владелец: {owner_name}")
                    self.cell_dev_var.set(dev_info if dev_info else "")
                    
                    if config["type"] == "property" and cell_idx in self.rent_tables:
                        rent_table = self.rent_tables[cell_idx]
                        if prop_info["hotel"]:
                            current_rent = rent_table[5]
                            rent_info = f"Текущая аренда: ${current_rent} (с отелем)"
                        elif prop_info["houses"] > 0:
                            current_rent = rent_table[prop_info["houses"]]
                            rent_info = f"Текущая аренда: ${current_rent} (с {prop_info['houses']} домом(ами))"
                        else:
                            current_rent = rent_table[0]
                            rent_info = f"Текущая аренда: ${current_rent}"
                        
                        rent_prog = f"Прогрессия аренды: ${rent_table[0]} → ${rent_table[1]} → ${rent_table[2]} → ${rent_table[3]} → ${rent_table[4]} → ${rent_table[5]} (отель)"
                        self.cell_rent_var.set(rent_info + "\n" + rent_prog)
                    else:
                        self.cell_rent_var.set("")
                else:
                    self.cell_owner_var.set("Доступно для покупки")
                    self.cell_dev_var.set("")
                    self.cell_rent_var.set("")
            else:
                self.cell_owner_var.set("")
                self.cell_dev_var.set("")
                self.cell_rent_var.set("")
            
            if config["type"] == "property" and config["color"] in self.color_groups:
                costs = self.color_groups[config["color"]]
                self.cell_build_cost_var.set(f"Стоимость строительства: 🏠 ${costs['house_cost']} каждый | 🏨 ${costs['hotel_cost']}")
                self.build_costs_var.set(f"Текущая собственность: 🏠 ${costs['house_cost']} | 🏨 ${costs['hotel_cost']}")
            else:
                self.cell_build_cost_var.set("")
                self.build_costs_var.set("")
                
            self.update_development_info()

    def update_development_info(self):
        """Update development information"""
        player = self.players[self.current_player]
        dev_text = ""
        
        owned_properties = []
        for idx, info in self.property_owners.items():
            if info and isinstance(info, dict) and info.get("owner") == player["name"]:
                owned_properties.append(idx)
        
        if owned_properties:
            dev_text = f"Владеет {len(owned_properties)} собственностями"
            total_houses = sum(self.property_owners[idx].get("houses", 0) for idx in owned_properties 
                             if self.property_owners.get(idx))
            total_hotels = sum(1 for idx in owned_properties 
                             if self.property_owners.get(idx) and self.property_owners[idx].get("hotel"))
            
            if total_houses > 0 or total_hotels > 0:
                dev_text += f" | 🏠:{total_houses} 🏨:{total_hotels}"
        
        self.dev_info_var.set(dev_text)

    def roll_dice(self):
        """Roll dice and move player"""
        current_player = self.players[self.current_player]
        
        if current_player["in_jail"]:
            messagebox.showwarning("В тюрьме", "Вы в тюрьме! Заплатите $50, чтобы выйти.")
            return
            
        # Генерируем значения кубиков
        dice1 = random.randint(1, 6)
        dice2 = random.randint(1, 6)
        total = dice1 + dice2
        
        # Анимируем бросок кубиков
        def after_animation():
            self.move_player(total)
            # Скрываем анимацию через 2 секунды
            self.canvas.after(2000, self.dice_animation.hide)
        
        self.dice_animation.roll(dice1, dice2, after_animation)

    def move_player(self, steps):
        """Move player"""
        player = self.players[self.current_player]
        old_position = player["position"]
        
        new_position = old_position + steps
        
        # Проверяем прохождение GO
        circles_passed = 0
        if new_position >= 40:
            circles_passed = new_position // 40
            new_position = new_position % 40
            player["money"] += 200 * circles_passed
            self.go_passed_count += circles_passed
            
            messagebox.showinfo("GO!", 
                              f"Вы прошли GO {circles_passed} раз(а) и получаете ${200 * circles_passed}!\n"
                              f"Всего GO пройдено: {self.go_passed_count} раз")
            
            # Автосохранение при прохождении GO
            self.save_game(autosave=True)
        
        player["position"] = new_position
        
        self.draw_players()
        self.on_cell_click(new_position)
        self.update_player_info()
        
        self.check_special_cell(new_position)

    def check_special_cell(self, position):
        """Handle special cell"""
        config = self.cell_configs[position]
        player = self.players[self.current_player]
        
        if config["name"] == "GO":
            player["money"] += 200
            self.update_player_info()
            messagebox.showinfo("GO!", "Вы приземлились на GO! Получите $200!")
        elif config["type"] == "tax":
            tax_amount = config["price"]
            if player["money"] >= tax_amount:
                player["money"] -= tax_amount
                self.update_player_info()
                messagebox.showinfo("Налог", f"Вы платите налог: ${tax_amount}")
            else:
                messagebox.showwarning("Банкротство", f"У вас недостаточно денег, чтобы заплатить налог ${tax_amount}!")
        elif config["type"] == "chance":
            self.draw_chance_card()
        elif config["type"] == "community":
            self.draw_community_chest_card()
        elif config["name"] == "Go To Jail":
            self.send_to_jail()
        elif config["type"] in ["property", "railroad", "utility"]:
            owner_info = self.property_owners.get(position)
            if owner_info and owner_info["owner"] and owner_info["owner"] != player["name"]:
                rent = self.calculate_rent(position, owner_info)
                
                if player["money"] >= rent:
                    player["money"] -= rent
                    for p in self.players:
                        if p["name"] == owner_info["owner"]:
                            p["money"] += rent
                            break
                    
                    rent_details = f"Вы платите аренду {owner_info['owner']}: ${rent}"
                    if config["type"] == "property":
                        if owner_info["hotel"]:
                            rent_details += f" (аренда с отелем)"
                        elif owner_info["houses"] > 0:
                            rent_details += f" ({owner_info['houses']} дом(ов))"
                    
                    messagebox.showinfo("Аренда", rent_details)
                    self.update_player_info()
                else:
                    messagebox.showwarning("Банкротство", f"У вас недостаточно денег, чтобы заплатить аренду ${rent}!")

    def calculate_rent(self, position, owner_info):
        """Calculate rent"""
        config = self.cell_configs[position]
        
        if config["type"] == "railroad":
            owner_name = owner_info["owner"]
            owned_railroads = sum(1 for idx in [5, 15, 25, 35] 
                                if self.property_owners[idx] and self.property_owners[idx]["owner"] == owner_name)
            rent = 25 * (2 ** (owned_railroads - 1)) if owned_railroads > 0 else 25
            return rent
            
        elif config["type"] == "utility":
            owner_name = owner_info["owner"]
            owned_utilities = sum(1 for idx in [12, 28] 
                                if self.property_owners[idx] and self.property_owners[idx]["owner"] == owner_name)
            dice_roll = random.randint(1, 6) + random.randint(1, 6)
            rent = dice_roll * (4 if owned_utilities == 1 else 10)
            return rent
            
        elif config["type"] == "property":
            if position in self.rent_tables:
                rent_table = self.rent_tables[position]
                if owner_info["hotel"]:
                    return rent_table[5]
                elif owner_info["houses"] > 0:
                    houses = min(owner_info["houses"], 4)
                    return rent_table[houses]
                else:
                    return rent_table[0]
        
        return int(config["price"] * 0.1)

    def send_to_jail(self):
        """Send to jail"""
        player = self.players[self.current_player]
        player["position"] = 10
        player["in_jail"] = True
        player["jail_turns"] = 0
        self.draw_players()
        self.update_player_info()
        messagebox.showinfo("Тюрьма", "Вы отправлены в тюрьму!")

    def pay_jail_fine(self):
        """Pay jail fine"""
        player = self.players[self.current_player]
        if not player["in_jail"]:
            messagebox.showinfo("Информация", "Вы не в тюрьме!")
            return
            
        if player["money"] >= 50:
            player["money"] -= 50
            player["in_jail"] = False
            player["jail_turns"] = 0
            self.update_player_info()
            self.draw_players()
            messagebox.showinfo("Тюрьма", "Вы заплатили $50 и теперь свободны!")
        else:
            messagebox.showwarning("Ошибка", "Недостаточно денег для оплаты штрафа!")

    def draw_chance_card(self):
        card = random.choice(self.chance_cards)
        messagebox.showinfo("Шанс", card["text"])
        self.process_card_action(card)

    def draw_community_chest_card(self):
        card = random.choice(self.community_chest_cards)
        messagebox.showinfo("Общественная казна", card["text"])
        self.process_card_action(card)

    def process_card_action(self, card):
        player = self.players[self.current_player]
        if card["action"] == "add_money":
            player["money"] += card["amount"]
            messagebox.showinfo("Получены деньги", f"Вы получаете ${card['amount']}!")
        elif card["action"] == "subtract_money":
            player["money"] -= card["amount"]
            messagebox.showinfo("Оплата", f"Вы платите ${card['amount']}!")
        elif card["action"] == "go_to_go":
            player["position"] = 0
            player["money"] += 200
            self.draw_players()
            messagebox.showinfo("Перемещение", "Вы перемещаетесь на GO и получаете $200!")
            
            # Прохождение GO - автосохранение
            self.go_passed_count += 1
            self.save_game(autosave=True)
            
        elif card["action"] == "go_to_jail":
            self.send_to_jail()
        self.update_player_info()

    def buy_property(self):
        """Buy property"""
        player = self.players[self.current_player]
        if player["in_jail"]:
            messagebox.showwarning("В тюрьме", "Вы не можете покупать собственность, находясь в тюрьме!")
            return
            
        position = player["position"]
        config = self.cell_configs[position]
        
        if config["type"] not in ["property", "railroad", "utility"]:
            messagebox.showwarning("Ошибка", "Вы не можете купить эту собственность!")
            return
        
        prop_info = self.property_owners[position]
        if prop_info["owner"] is not None:
            messagebox.showwarning("Ошибка", "Эта собственность уже куплена!")
            return
        
        if player["money"] < config["price"]:
            messagebox.showwarning("Ошибка", "Недостаточно денег!")
            return
            
        player["money"] -= config["price"]
        prop_info["owner"] = player["name"]
        player["properties"].append(position)
        self.update_player_info()
        self.on_cell_click(position)
        self.update_development_info()
        
        self.draw_players()
        self.update_display_for_cell(position)
        
        #Обновляем список купленных улиц
        self.update_owned_streets_list()
        
        messagebox.showinfo("Покупка", f"Вы купили {config['name']} за ${config['price']}!")

    def update_display_for_cell(self, cell_idx):
        """Update cell display"""
        if cell_idx < len(self.cells):
            cell = self.cells[cell_idx]
            x1, y1, x2, y2 = cell["coords"]
            
            if cell_idx in self.property_owners:
                prop_info = self.property_owners[cell_idx]
                if prop_info and prop_info["owner"]:
                    items = self.canvas.find_overlapping(x1, y1, x2, y2)
                    for item in items:
                        tags = self.canvas.gettags(item)
                        if "house" in tags:
                            self.canvas.delete(item)
                    
                    if prop_info["houses"] > 0 or prop_info["hotel"]:
                        if prop_info["hotel"]:
                            self.canvas.create_text(x1 + (x2 - x1) // 2, y1 + 25,
                                                  text="🏨", font=("Arial", 16),
                                                  fill="black", tags="house")
                        elif prop_info["houses"] > 0:
                            house_text = "🏠" * min(prop_info["houses"], 4)
                            self.canvas.create_text(x1 + (x2 - x1) // 2, y1 + 25,
                                                  text=house_text, font=("Arial", 12),
                                                  fill="black", tags="house")

    def can_build_on_property(self, position, houses_to_build=1):
        """Check if can build"""
        player = self.players[self.current_player]
        config = self.cell_configs[position]
        
        prop_info = self.property_owners[position]
        if not prop_info or prop_info["owner"] != player["name"]:
            return False, "Вы не владеете этой собственностью!"
        
        if config["type"] != "property":
            return False, "Вы можете строить только на собственности!"
        
        if prop_info["hotel"]:
            return False, "На собственности уже есть отель!"
        
        color = config["color"]
        if color not in self.color_groups:
            return False, "Этот цвет собственности не позволяет строительство!"
        
        color_group = self.color_groups[color]
        
        for prop_idx in color_group["properties"]:
            prop_owner_info = self.property_owners[prop_idx]
            if not prop_owner_info or prop_owner_info["owner"] != player["name"]:
                return False, f"Сначала нужно владеть всеми {color} свойствами!"
        
        # НОВЫЕ ПРАВИЛА: разрешаем строить сразу несколько домов
        # Проверяем, не превышаем ли лимит в 4 дома
        if prop_info["houses"] + houses_to_build > 4:
            return False, f"Нельзя построить {houses_to_build} дом(ов). Максимум 4 дома на собственности!"
        
        # Проверяем равномерность строительства (можно строить больше, чем на других свойствах)
        min_houses_in_group = min(self.property_owners[idx]["houses"] 
                                for idx in color_group["properties"])
        
        # Разрешаем строить даже если на других свойствах меньше домов
        # (убрали ограничение на равномерность)
        
        return True, ""

    def build_house(self):
        """Build house"""
        self.build_multiple_houses(1)

    def build_multiple_houses(self, houses_to_build=1, window=None):
        """Build multiple houses at once"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        can_build, message = self.can_build_on_property(position, houses_to_build)
        if not can_build:
            messagebox.showwarning("Нельзя построить", message)
            if window:
                window.destroy()
            return
        
        prop_info = self.property_owners[position]
        
        color = config["color"]
        house_cost = self.color_groups[color]["house_cost"]
        total_cost = house_cost * houses_to_build
        
        if player["money"] < total_cost:
            messagebox.showwarning("Нельзя построить", 
                                 f"Недостаточно денег! {houses_to_build} дом(ов) стоит ${total_cost}")
            if window:
                window.destroy()
            return
        
        # Строим все дома сразу
        old_houses = prop_info["houses"]
        prop_info["houses"] += houses_to_build
        player["money"] -= total_cost
        
        self.update_player_info()
        self.on_cell_click(position)
        self.update_development_info()
        
        self.update_display_for_cell(position)
        self.draw_players()
        
        #Обновляем список купленных улиц
        self.update_owned_streets_list()
        
        if position in self.rent_tables:
            old_rent = self.rent_tables[position][old_houses]
            new_rent = self.rent_tables[position][prop_info["houses"]]
            messagebox.showinfo("Дома построены", 
                              f"Вы построили {houses_to_build} дом(ов) на {config['name']} за ${total_cost}!\n"
                              f"Теперь {prop_info['houses']} дом(ов).\n"
                              f"Аренда увеличилась с ${old_rent} до ${new_rent}.")
        
        if window:
            window.destroy()

    def build_hotel(self):
        """Build hotel"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        can_build, message = self.can_build_on_property(position)
        if not can_build:
            messagebox.showwarning("Нельзя построить", message)
            return
        
        prop_info = self.property_owners[position]
        
        if prop_info["houses"] < 4:
            messagebox.showwarning("Нельзя построить", "Собственности нужно 4 дома перед постройкой отеля!")
            return
        
        if prop_info["hotel"]:
            messagebox.showwarning("Нельзя построить", "На собственности уже есть отель!")
            return
        
        color = config["color"]
        hotel_cost = self.color_groups[color]["hotel_cost"]
        
        if player["money"] < hotel_cost:
            messagebox.showwarning("Нельзя построить", f"Недостаточно денег! Отель стоит ${hotel_cost}")
            return
        
        player["money"] -= hotel_cost
        prop_info["hotel"] = True
        old_houses = prop_info["houses"]
        prop_info["houses"] = 0
        self.update_player_info()
        self.on_cell_click(position)
        self.update_development_info()
        
        self.update_display_for_cell(position)
        self.draw_players()
        
        #Обновляем список купленных улиц
        self.update_owned_streets_list()
        
        if position in self.rent_tables:
            old_rent = self.rent_tables[position][4]
            new_rent = self.rent_tables[position][5]
            messagebox.showinfo("Отель построен", 
                              f"Вы построили отель на {config['name']} за ${hotel_cost}!\n"
                              f"4 дома заменены на 1 отель.\n"
                              f"Аренда увеличилась с ${old_rent} до ${new_rent}.")

    def sell_house(self):
        """Sell house"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        prop_info = self.property_owners[position]
        if not prop_info or prop_info["owner"] != player["name"]:
            messagebox.showwarning("Нельзя продать", "Вы не владеете этой собственностью!")
            return
        
        if prop_info["hotel"]:
            messagebox.showwarning("Нельзя продать", "Сначала продайте отель!")
            return
        
        if prop_info["houses"] == 0:
            messagebox.showwarning("Нельзя продать", "На собственности нет домов для продажи!")
            return
        
        color = config["color"]
        color_group = self.color_groups[color]
        
        for prop_idx in color_group["properties"]:
            prop_owner_info = self.property_owners[prop_idx]
            if not prop_owner_info or prop_owner_info["owner"] != player["name"]:
                messagebox.showwarning("Нельзя продать", f"Вы должны владеть всеми {color} свойствами, чтобы продавать дома!")
                return
        
        # Новые правила: можно продавать дома независимо от других свойств
        max_houses_in_group = max(self.property_owners[idx]["houses"] 
                                for idx in color_group["properties"])
        
        # Разрешаем продавать даже если у других свойств больше домов
        # (убрали ограничение на равномерность продажи)
        
        sell_price = self.color_groups[color]["house_cost"] // 2
        
        player["money"] += sell_price
        old_houses = prop_info["houses"]
        prop_info["houses"] -= 1
        self.update_player_info()
        self.on_cell_click(position)
        self.update_development_info()
        
        self.update_display_for_cell(position)
        self.draw_players()
        
        #Обновляем список купленных улиц
        self.update_owned_streets_list()
        
        if position in self.rent_tables:
            old_rent = self.rent_tables[position][old_houses]
            new_rent = self.rent_tables[position][prop_info["houses"]]
            messagebox.showinfo("Дом продан", 
                              f"Вы продали дом с {config['name']} за ${sell_price}!\n"
                              f"Теперь {prop_info['houses']} дом(ов).\n"
                              f"Аренда уменьшилась с ${old_rent} до ${new_rent}.")

    def sell_hotel(self):
        """Sell hotel"""
        player = self.players[self.current_player]
        position = player["position"]
        config = self.cell_configs[position]
        
        prop_info = self.property_owners[position]
        if not prop_info or prop_info["owner"] != player["name"]:
            messagebox.showwarning("Нельзя продать", "Вы не владеете этой собственностью!")
            return
        
        if not prop_info["hotel"]:
            messagebox.showwarning("Нельзя продать", "На собственности нет отеля для продажи!")
            return
        
        color = config["color"]
        color_group = self.color_groups[color]
        
        for prop_idx in color_group["properties"]:
            prop_owner_info = self.property_owners[prop_idx]
            if not prop_owner_info or prop_owner_info["owner"] != player["name"]:
                messagebox.showwarning("Нельзя продать", f"Вы должны владеть всеми {color} свойствами, чтобы продавать отель!")
                return
        
        sell_price = self.color_groups[color]["hotel_cost"] // 2
        
        player["money"] += sell_price
        prop_info["hotel"] = False
        prop_info["houses"] = 4
        self.update_player_info()
        self.on_cell_click(position)
        self.update_development_info()
        
        self.update_display_for_cell(position)
        self.draw_players()
        
        #Обновляем список купленных улиц
        self.update_owned_streets_list()
        
        if position in self.rent_tables:
            old_rent = self.rent_tables[position][5]
            new_rent = self.rent_tables[position][4]
            messagebox.showinfo("Отель продан", 
                              f"Вы продали отель на {config['name']} за ${sell_price}!\n"
                              f"Отель преобразован обратно в 4 дома.\n"
                              f"Аренда уменьшилась с ${old_rent} до ${new_rent}.")

    def next_turn(self):
        """Next turn"""
        self.current_player = (self.current_player + 1) % len(self.players)
        self.update_player_info()
        self.draw_players()
        if not self.auto_mode:
            messagebox.showinfo("Смена хода", f"Сейчас ходит {self.players[self.current_player]['name']}")

    def update_player_info(self):
        """Update player info"""
        player = self.players[self.current_player]
        self.player_var.set(player["name"])
        self.money_var.set(f"Деньги: ${player['money']}")
        
        if player["in_jail"]:
            self.jail_var.set(f"В ТЮРЬМЕ")
        else:
            self.jail_var.set("")
        
        self.go_counter_var.set(f"GO пройдено: {self.go_passed_count} раз")
        
        self.update_development_info()
        self.draw_center_area()

#меню
def start_new_game():
    """Запустить новую игру"""
    root = tk.Tk()
    app = MonopolyBoard(root)
    root.mainloop()

def continue_old_game(filename=None):
    """Продолжить старую игру"""
    root = tk.Tk()
    app = MonopolyBoard(root, load_from_file=filename)
    root.mainloop()

def create_start_menu():
    """Создать стартовое меню"""
    root = tk.Tk()
    menu = StartMenu(root, start_new_game, continue_old_game)
    root.mainloop()

if __name__ == "__main__":
    create_start_menu()