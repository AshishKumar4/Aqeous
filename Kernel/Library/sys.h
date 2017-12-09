#ifndef SYS_H
#define SYS_H


inline uint32_t readl(uint32_t addr);

inline void writel(uint32_t addr, uint32_t val);

void delay(uint32_t delay);

inline void outb(uint16_t port, uint8_t val);

inline uint8_t inb(uint16_t port);

inline void outw(uint16_t port, uint16_t val);

inline uint16_t inw(uint16_t port);

inline void outpd(uint16_t port, uint16_t val);

inline uint16_t inpd(uint16_t port);

inline void outl(uint16_t port, uint32_t val);

inline uint32_t inl(uint16_t port);

inline void io_wait(void);

static inline bool are_interrupts_enabled();

static inline void lidt_n(void* base, uint16_t size);

inline void sysManager(unsigned int todo); // 1 : reboot; 2 : shutdown; 3 : reserved


#endif // SYS_H
