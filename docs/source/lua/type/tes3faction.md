# tes3faction

A faction game object.

## Properties

<dl class="describe">
<dt><code class="descname">actorFlags: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

A number representing the actor flags. Truly a bit field.

</dd>
<dt><code class="descname">attributes: <a href="https://mwse.readthedocs.io/en/latest/lua/type/table.html">table</a></code></dt>
<dd>

An array-style table holding the two attributes that govern advancement.

</dd>
<dt><code class="descname">boundingBox: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3boundingBox.html">tes3boundingBox</a></code></dt>
<dd>

The bounding box for the object.

</dd>
<dt><code class="descname">cloneCount: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The number of clones that exist of this actor.

</dd>
<dt><code class="descname">deleted: <a href="https://mwse.readthedocs.io/en/latest/lua/type/boolean.html">boolean</a></code></dt>
<dd>

The deleted state of the object.

</dd>
<dt><code class="descname">disabled: <a href="https://mwse.readthedocs.io/en/latest/lua/type/boolean.html">boolean</a></code></dt>
<dd>

The disabled state of the object.

</dd>
<dt><code class="descname">equipment: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3iterator.html">tes3iterator</a></code></dt>
<dd>

The items currently equipped to the actor.

</dd>
<dt><code class="descname">id: <a href="https://mwse.readthedocs.io/en/latest/lua/type/string.html">string</a></code></dt>
<dd>

The unique identifier for the object.

</dd>
<dt><code class="descname">inventory: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3iterator.html">tes3iterator</a></code></dt>
<dd>

The items currently carried by the actor.

</dd>
<dt><code class="descname">modified: <a href="https://mwse.readthedocs.io/en/latest/lua/type/boolean.html">boolean</a></code></dt>
<dd>

The modification state of the object since the last save.

</dd>
<dt><code class="descname">name: <a href="https://mwse.readthedocs.io/en/latest/lua/type/string.html">string</a></code></dt>
<dd>

The faction's player-facing name.

</dd>
<dt><code class="descname">nextInCollection: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3object.html">tes3object</a></code></dt>
<dd>

The next object in parent collection's list.

</dd>
<dt><code class="descname">objectFlags: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The raw flags of the object.

</dd>
<dt><code class="descname">objectType: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The type of object. Maps to values in tes3.objectType.

</dd>
<dt><code class="descname">owningCollection: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3referenceList.html">tes3referenceList</a></code></dt>
<dd>

The collection responsible for holding this object.

</dd>
<dt><code class="descname">playerExpelled: <a href="https://mwse.readthedocs.io/en/latest/lua/type/boolean.html">boolean</a></code></dt>
<dd>

The player's expelled state in the faction.

</dd>
<dt><code class="descname">playerJoined: <a href="https://mwse.readthedocs.io/en/latest/lua/type/boolean.html">boolean</a></code></dt>
<dd>

The player's join state for the faction.

</dd>
<dt><code class="descname">playerRank: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The player's current rank in the faction.

</dd>
<dt><code class="descname">playerReputation: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The player's current reputation in the faction.

</dd>
<dt><code class="descname">previousInCollection: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3object.html">tes3object</a></code></dt>
<dd>

The previous object in parent collection's list.

</dd>
<dt><code class="descname">ranks: <a href="https://mwse.readthedocs.io/en/latest/lua/type/table.html">table</a></code></dt>
<dd>

An array-style table holding the ten related tes3factionRanks.

</dd>
<dt><code class="descname">reactions: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3iterator.html">tes3iterator</a></code></dt>
<dd>

A collection of tes3factionReactions.

</dd>
<dt><code class="descname">scale: <a href="https://mwse.readthedocs.io/en/latest/lua/type/number.html">number</a></code></dt>
<dd>

The object's scale.

</dd>
<dt><code class="descname">sceneNode: <a href="https://mwse.readthedocs.io/en/latest/lua/type/niNode.html">niNode</a></code></dt>
<dd>

The scene graph node for this object.

</dd>
<dt><code class="descname">sceneReference: <a href="https://mwse.readthedocs.io/en/latest/lua/type/niNode.html">niNode</a></code></dt>
<dd>

The scene graph reference node for this object.

</dd>
<dt><code class="descname">skills: <a href="https://mwse.readthedocs.io/en/latest/lua/type/table.html">table</a></code></dt>
<dd>

An array-style table holding the seven skills that govern advancement.

</dd>
<dt><code class="descname">sourceMod: <a href="https://mwse.readthedocs.io/en/latest/lua/type/string.html">string</a></code></dt>
<dd>

The filename of the mod that owns this object.

</dd>
<dt><code class="descname">stolenList: <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3iterator.html">tes3iterator</a></code></dt>
<dd>

A list of actors that the object has been stolen from.

</dd>
</dl>

## Methods

<dl class="describe">
<dt><code class="descname">onInventoryClose(<i>reference:</i> <a href="https://mwse.readthedocs.io/en/latest/lua/type/tes3reference.html">tes3reference</a>)</code></dt>
<dd>

A callback function invoked when an inventory is closed. Typically not used outside of specific purposes.

</dd>
</dl>

## Metatable Events

<dl class="describe">
<dt><code class="descname">__tostring</code></dt>
<dd>

An object can be converted using `tostring()` to its id.

</dd>
</dl>