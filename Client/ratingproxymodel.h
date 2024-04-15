#ifndef RATINGPROXYMODEL_H
#define RATINGPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class RatingProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RatingProxyModel(QObject *parent = nullptr);

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

};

#endif // RATINGPROXYMODEL_H
