class RoomNameValidator < ActiveModel::EachValidator
  def validate_each(record, attribute, value)
    unless /.+ #\d{2}\z/ === value
      record.errors.add :attribute, (options[:message] || "会議室名が正しくありません")
    end
  end
end
