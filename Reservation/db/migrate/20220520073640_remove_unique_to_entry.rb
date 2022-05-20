class RemoveUniqueToEntry < ActiveRecord::Migration[6.1]
  def change
    remove_index :entries, :user_email
  end
end
