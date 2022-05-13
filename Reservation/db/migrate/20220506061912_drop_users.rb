class DropUsers < ActiveRecord::Migration[6.1]
  def change
    drop_table :users do
    end
  end
end
