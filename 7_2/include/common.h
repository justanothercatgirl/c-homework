#ifndef JAC_COMMON_H
#define JAC_COMMON_H

/* Для чтающих:
 * чтобы писать 
 * `vector2 point = { 10, 20 };`
 * вместо
 * `struct vector2 point = { 10, 20 };`
 * (то есть чтобы писать struct перед vector2 было необязательно)
 * надо объявлять структуру так:
 * ```
 * typedef struct {
 * 	long x, y;
 * } vector2;
 * ```
 * Таким образом мы объявляем структуру без официального названия,
 * но присваиваем ей синоним. Чтобы пользоваться и тем, и тем вариантами,
 * можно сделать так:
 * ```
 * typedef struct vector2 {
 * 	long x, y;
 * } vector2;
 * ```
 * Так мы и даём ей название, и даём ей синоним.
 */

// координаты точки в 2-мерном пространстве
typedef struct vector2 {
	long x, y;
} vector2;

#endif // JAC_COMMON_H
