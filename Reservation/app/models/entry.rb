class Entry < ApplicationRecord
  scope :least_entries, ->(date) {where("reserved_date >= ?", date.to_date - 7.days).where("reserved_date <= ?", date.to_date + 7.days)}

  validates :user_name, :user_email, :reserved_date, :usage_time, :people, presence: true
  validate :email_check

  belongs_to :room

  private
    def email_check
        email_pattern = /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/
        unless email_pattern.match(self.user_email)
            errors.add(:user_email, "正しいメールアドレスを入力してください")
        end
    end
end
