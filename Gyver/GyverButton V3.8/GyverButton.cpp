#include "GyverButton.h"

//HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)

//https://alexgyver.ru/gyverbutton/

//butt1.tick();  // обязательная функция отработки. Должна постоянно опрашиваться
//if (butt1.isClick()) Serial.println("Click");         // проверка на один клик
//if (butt1.isSingle()) Serial.println("Single");       // проверка на один клик
//if (butt1.isDouble()) Serial.println("Double");       // проверка на двойной клик
//if (butt1.isTriple()) Serial.println("Triple");       // проверка на тройной клик
//if (butt1.hasClicks())                                // проверка на наличие нажатий
//Serial.println(butt1.getClicks());                  // получить (и вывести) число нажатий
//if (butt1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
//if (butt1.isRelease()) Serial.println("Release");     // отпускание кнопки (+ дебаунс)
//if (butt1.isHolded()) Serial.println("Holded");       // проверка на удержание
//if (butt1.isHold()) Serial.println("Holding");        // проверка на удержание
////if (butt1.state()) Serial.println("Hold");          // возвращает состояние кнопки
//if (butt1.isStep()) {                                 // если кнопка была удержана (это для инкремента)
//value++;                                            // увеличивать/уменьшать переменную value с шагом и интервалом
//Serial.println(value);                              // для примера выведем в порт
//}

// ==================== CONSTRUCTOR ====================
GButton::GButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, bool type, bool dir) {

	flags.GPIOx = GPIOx;
	flags.GPIO_Pin = GPIO_Pin;

	setType(type);
	flags.mode = false;
	flags.tickMode = false;
	flags.inv_state = dir;
}

// ==================== SET ====================
void GButton::setDebounce(uint16_t debounce) {
	_debounce = debounce;
}
void GButton::setTimeout(uint16_t new_timeout) {
	_timeout = new_timeout;
}
void GButton::setClickTimeout(uint16_t new_timeout) {
	_click_timeout = new_timeout;
}
void GButton::setStepTimeout(uint16_t step_timeout) {
	_step_timeout = step_timeout;
}
void GButton::setType(bool type) {
	flags.type = type;
}
void GButton::setDirection(bool dir) {
	flags.inv_state = dir;
}
void GButton::setTickMode(bool tickMode) {
	flags.tickMode = tickMode;
}

// ==================== IS ====================
bool GButton::isPress() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.isPress_f) {
		flags.isPress_f = false;
		return true;
	} else
		return false;
}
bool GButton::isRelease() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.isRelease_f) {
		flags.isRelease_f = false;
		return true;
	} else
		return false;
}


bool GButton::isClick() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.isOne_f) {
		flags.isOne_f = false;
		return true;
	} else
		return false;
}
bool GButton::isHolded() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.isHolded_f) {
		flags.isHolded_f = false;
		return true;
	} else
		return false;
}

bool GButton::isHold() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.step_flag)
		return true;
	else
		return false;
}
bool GButton::state() {
	if (flags.tickMode)
		GButton::tick();
	return btn_state;
}




bool GButton::isSingle() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.counter_flag && last_counter == 1) {
		//		last_counter = 0;
		//		flags.counter_flag = false;
		flags.counter_reset = true;
		return true;
	} else
		return false;
}
bool GButton::isDouble() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.counter_flag && last_counter == 2) {
		//		flags.counter_flag = false;
		//		last_counter = 0;
		flags.counter_reset = true;
		return true;
	} else
		return false;
}
bool GButton::isTriple() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.counter_flag && last_counter == 3) {
		//		flags.counter_flag = false;
		//		last_counter = 0;
		flags.counter_reset = true;
		return true;
	} else
		return false;
}



bool GButton::hasClicks() {
	if (flags.tickMode)
		GButton::tick();
	if (flags.counter_flag) {
		//		flags.counter_flag = false;
		flags.counter_reset = true;
		return true;
	} else
		return false;
}
uint8_t GButton::getClicks() {
	//	byte thisCount = last_counter;			// Исключено	14.01.2021
	//		last_counter = 0;
	flags.counter_reset = true;
	return last_counter;		//	return thisCount;	(замена)	14.01.2021
}
uint8_t GButton::getHoldClicks() {
	if (flags.tickMode)
		GButton::tick();
	return last_hold_counter;//return flags.hold_flag ? last_hold_counter : 0;	(Замена)	 14.01.2021
}



bool GButton::isStep(uint8_t clicks) {
	if (flags.tickMode)
		GButton::tick();
	if (btn_counter == clicks && flags.step_flag
			&& (uwTick - btn_timer >= _step_timeout)) {
		btn_timer = uwTick;
		return true;
	} else
		return false;
}

void GButton::resetStates() {
	flags.isPress_f = false;
	flags.isRelease_f = false;
	flags.isOne_f = false;
	flags.isHolded_f = false;
	flags.step_flag = false;
	flags.counter_flag = false;
	last_hold_counter = 0;
	last_counter = 0;
}

// ==================== TICK ====================
void GButton::tick(bool state) {
	flags.mode = true;
	btn_state = state ^ flags.inv_state;
	GButton::tick();
	flags.mode = false;
}

void GButton::tick() {
	// читаем пин
	//if (!flags.mode && !flags.noPin) btn_state = !_buttonRead() ^ (flags.inv_state ^ flags.type);

	if (!flags.mode)	// && !flags.noPin)
		btn_state = !HAL_GPIO_ReadPin(flags.GPIOx, flags.GPIO_Pin)
				^ (flags.inv_state ^ flags.type);

	uint32_t thisMls = uwTick;
	//uint32_t thisMls = millis();

	// нажатие
	if (btn_state && !btn_flag) {
		if (!flags.btn_deb) {
			flags.btn_deb = true;
			btn_timer = thisMls;
		} else {
			if (thisMls - btn_timer >= _debounce) {
				btn_flag = true;
				flags.isPress_f = true;
				flags.oneClick_f = true;
			}
		}
	} else {
		flags.btn_deb = false;
	}

	// отпускание
	if (!btn_state && btn_flag) {
		btn_flag = false;
		if (!flags.hold_flag)
			btn_counter++;
		flags.hold_flag = false;
		flags.isRelease_f = true;
		btn_timer = thisMls;
		if (flags.step_flag) {
			last_counter = 0;
			btn_counter = 0;
			flags.step_flag = false;
		}
		if (flags.oneClick_f) {
			flags.oneClick_f = false;
			flags.isOne_f = true;
		}
	}

	// кнопка удерживается
	if (btn_flag && btn_state && (thisMls - btn_timer >= _timeout)
			&& !flags.hold_flag) {
		flags.hold_flag = true;
		last_hold_counter = btn_counter;
		//btn_counter = 0;
		//last_counter = 0;
		flags.isHolded_f = true;
		flags.step_flag = true;
		flags.oneClick_f = false;
		btn_timer = thisMls;
	}

	// обработка накликивания
	if ((thisMls - btn_timer >= _click_timeout) && (btn_counter != 0)
			&& !btn_state) {    //И здесь еще добавлен !btn_state
		last_counter = btn_counter;
		btn_counter = 0;
		flags.counter_flag = true;
	}

	// сброс накликивания						//Добавлено
	if (flags.counter_reset) {
		last_counter = 0;
		flags.counter_flag = false;
		flags.counter_reset = false;
	}
}
