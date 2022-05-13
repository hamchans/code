class DropUsersEmail < ActiveRecord::Migration[6.1]
  def change
    drop_table :users do |t|
      t.string :email
    end
  end
end
