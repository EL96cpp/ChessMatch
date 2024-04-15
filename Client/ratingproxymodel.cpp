#include "ratingproxymodel.h"

RatingProxyModel::RatingProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

bool RatingProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {

    QVariant left_variant_value = sourceModel()->data(source_left);
    QVariant right_variant_value = sourceModel()->data(source_right);

    int left_value = left_variant_value.toInt();
    int right_value = right_variant_value.toInt();

    qDebug() << "Comparing " << left_value << " and " << right_value;

    return left_value < right_value;

}

