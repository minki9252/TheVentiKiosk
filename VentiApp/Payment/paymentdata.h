#ifndef PAYMENTDATA_H
#define PAYMENTDATA_H
#include <QString>

struct PaymentData {
    int totalAmount = 0;       // 원래 총액
    int couponDiscount = 0;    // 쿠폰 할인액
    int finalAmount = 0;       // 최종 결제액

    QString phoneNumber = "";  // 적립할 번호
    QString orderNumber = "";  // 발급받은 주문/대기번호
    bool isReceiptWanted = false; // 영수증 출력 여부
};
#endif // PAYMENTDATA_H
